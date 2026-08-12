import torch
import torch.nn as nn
from transformers import AutoModel

try:
    from torchcrf import CRF as _CRF

    def make_crf(num_tags):
        return _CRF(num_tags, batch_first=True)

    def crf_loss(crf, emissions, tags, mask):
        return -crf(emissions, tags, mask=mask, reduction='mean')

    def crf_decode(crf, emissions, mask):
        return crf.decode(emissions, mask=mask)
except ModuleNotFoundError:
    from TorchCRF import CRF as _CRF

    def make_crf(num_tags):
        return _CRF(num_tags)

    def crf_loss(crf, emissions, tags, mask):
        return -crf(emissions, tags, mask).mean()

    def crf_decode(crf, emissions, mask):
        return crf.viterbi_decode(emissions, mask)

class BertCWS(nn.Module):
    def __init__(self, pretrained_model_name='hfl/chinese-roberta-wwm-ext', num_tags=4, dropout=0.2):
        super(BertCWS, self).__init__()
        self.bert = AutoModel.from_pretrained(pretrained_model_name)
        self.dropout = nn.Dropout(dropout)
        self.hidden2tag = nn.Linear(768, num_tags)
        self.crf = make_crf(num_tags)

    def forward(self, input_ids, attention_mask, tags):
        # input_ids: (batch_size, seq_len)
        # attention_mask: (batch_size, seq_len)
        # tags: (batch_size, seq_len)
        
        outputs = self.bert(input_ids, attention_mask=attention_mask)
        sequence_output = outputs.last_hidden_state  # (batch_size, seq_len, 768)
        sequence_output = self.dropout(sequence_output)
        
        emissions = self.hidden2tag(sequence_output)  # (batch_size, seq_len, num_tags)
        
        # Exclude PAD (0), CLS (101), and SEP (102) from CRF modeling mask
        crf_mask = (input_ids != 0) & (input_ids != 101) & (input_ids != 102)
        
        batch_size = emissions.size(0)
        device = emissions.device
        valid_lengths = crf_mask.sum(dim=1)
        max_valid_len = valid_lengths.max().item()
        if max_valid_len == 0:
            max_valid_len = 1
            
        # Allocate padded tensors starting at index 0 for CRF
        packed_emissions = torch.zeros(batch_size, max_valid_len, emissions.size(2), device=device)
        packed_tags = torch.zeros(batch_size, max_valid_len, dtype=torch.long, device=device)
        packed_mask = torch.zeros(batch_size, max_valid_len, dtype=torch.bool, device=device)
        
        for b in range(batch_size):
            valid_idx = crf_mask[b]
            num_valid = valid_lengths[b].item()
            if num_valid > 0:
                packed_emissions[b, :num_valid] = emissions[b, valid_idx]
                # Tags corresponding to valid characters (guaranteed to be valid B/M/E/S tags, no -100)
                packed_tags[b, :num_valid] = tags[b, valid_idx]
                packed_mask[b, :num_valid] = True
            else:
                packed_mask[b, 0] = True
                
        loss = crf_loss(self.crf, packed_emissions, packed_tags, packed_mask)
        return loss

    def infer(self, input_ids, attention_mask):
        outputs = self.bert(input_ids, attention_mask=attention_mask)
        sequence_output = outputs.last_hidden_state
        emissions = self.hidden2tag(sequence_output)
        
        crf_mask = (input_ids != 0) & (input_ids != 101) & (input_ids != 102)
        
        batch_size = emissions.size(0)
        device = emissions.device
        valid_lengths = crf_mask.sum(dim=1)
        max_valid_len = valid_lengths.max().item()
        if max_valid_len == 0:
            max_valid_len = 1
            
        packed_emissions = torch.zeros(batch_size, max_valid_len, emissions.size(2), device=device)
        packed_mask = torch.zeros(batch_size, max_valid_len, dtype=torch.bool, device=device)
        
        for b in range(batch_size):
            valid_idx = crf_mask[b]
            num_valid = valid_lengths[b].item()
            if num_valid > 0:
                packed_emissions[b, :num_valid] = emissions[b, valid_idx]
                packed_mask[b, :num_valid] = True
            else:
                packed_mask[b, 0] = True
                
        # Decode and return character-level predictions
        packed_predictions = crf_decode(self.crf, packed_emissions, packed_mask)
        
        # Trim predictions to actual character lengths for each sentence
        trimmed_predictions = []
        for b in range(batch_size):
            num_valid = valid_lengths[b].item()
            trimmed_predictions.append(packed_predictions[b][:num_valid])
            
        return trimmed_predictions
