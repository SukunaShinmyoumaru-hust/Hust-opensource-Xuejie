import torch
import torch.nn as nn
from transformers import AutoModel
from torch.nn.utils.rnn import pack_padded_sequence, pad_packed_sequence

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

class BertNER(nn.Module):
    def __init__(self, pretrained_model_name='hfl/chinese-roberta-wwm-ext', num_tags=21,
                 lstm_hidden=512, lstm_layers=2, dropout=0.3):
        """
        BERT + BiLSTM + CRF 命名实体识别模型
        Architecture: RoBERTa -> BiLSTM -> Dropout -> Linear -> CRF
        
        Args:
            pretrained_model_name: 预训练模型名称 (默认使用哈工大RoBERTa)
            num_tags: NER标签数量 (BIES×5类实体 + O = 21)
            lstm_hidden: BiLSTM 每个方向的隐藏维度 (双向输出 = lstm_hidden * 2)
            lstm_layers: BiLSTM 层数
            dropout: Dropout概率
        """
        super(BertNER, self).__init__()
        self.bert = AutoModel.from_pretrained(pretrained_model_name)
        
        # BiLSTM: 输入768(BERT输出) → 双向输出 lstm_hidden*2
        self.lstm = nn.LSTM(
            input_size=768,
            hidden_size=lstm_hidden,
            num_layers=lstm_layers,
            batch_first=True,
            bidirectional=True,
            dropout=dropout if lstm_layers > 1 else 0.0  # 多层LSTM之间的dropout
        )
        
        self.dropout = nn.Dropout(dropout)
        self.hidden2tag = nn.Linear(lstm_hidden * 2, num_tags)  # 双向拼接
        self.crf = make_crf(num_tags)

    def _get_features(self, input_ids, attention_mask):
        """BERT编码 → BiLSTM → 发射分数"""
        outputs = self.bert(input_ids, attention_mask=attention_mask)
        sequence_output = outputs.last_hidden_state  # (batch, seq_len, 768)
        
        # CRF mask: 排除 PAD(0), CLS(101), SEP(102)
        crf_mask = (input_ids != 0) & (input_ids != 101) & (input_ids != 102)
        
        batch_size = sequence_output.size(0)
        device = sequence_output.device
        valid_lengths = crf_mask.sum(dim=1)  # 每个样本的有效字符数
        max_valid_len = valid_lengths.max().item()
        if max_valid_len == 0:
            max_valid_len = 1
        
        # 提取有效字符的 BERT 输出，pack 后送入 BiLSTM
        packed_bert = torch.zeros(batch_size, max_valid_len, 768, device=device)
        for b in range(batch_size):
            valid_idx = crf_mask[b]
            num_valid = valid_lengths[b].item()
            if num_valid > 0:
                packed_bert[b, :num_valid] = sequence_output[b, valid_idx]
        
        # BiLSTM 前向 (使用 pack_padded_sequence 提高效率)
        lengths_cpu = valid_lengths.clamp(min=1).cpu()
        packed_input = pack_padded_sequence(packed_bert, lengths_cpu, batch_first=True, enforce_sorted=False)
        lstm_out, _ = self.lstm(packed_input)
        lstm_out, _ = pad_packed_sequence(lstm_out, batch_first=True, total_length=max_valid_len)
        
        # Dropout → Linear → 发射分数
        lstm_out = self.dropout(lstm_out)
        emissions = self.hidden2tag(lstm_out)  # (batch, max_valid_len, num_tags)
        
        # 构建 packed mask
        packed_mask = torch.zeros(batch_size, max_valid_len, dtype=torch.bool, device=device)
        for b in range(batch_size):
            num_valid = valid_lengths[b].item()
            if num_valid > 0:
                packed_mask[b, :num_valid] = True
            else:
                packed_mask[b, 0] = True
        
        return emissions, packed_mask, valid_lengths, crf_mask

    def forward(self, input_ids, attention_mask, tags):
        emissions, packed_mask, valid_lengths, crf_mask = self._get_features(input_ids, attention_mask)
        
        batch_size = tags.size(0)
        device = tags.device
        max_valid_len = emissions.size(1)
        
        # 提取有效字符的标签
        packed_tags = torch.zeros(batch_size, max_valid_len, dtype=torch.long, device=device)
        for b in range(batch_size):
            valid_idx = crf_mask[b]
            num_valid = valid_lengths[b].item()
            if num_valid > 0:
                packed_tags[b, :num_valid] = tags[b, valid_idx]
        
        loss = crf_loss(self.crf, emissions, packed_tags, packed_mask)
        return loss

    def infer(self, input_ids, attention_mask):
        emissions, packed_mask, valid_lengths, _ = self._get_features(input_ids, attention_mask)
        
        packed_predictions = crf_decode(self.crf, emissions, packed_mask)
        
        # 裁剪到实际字符长度
        trimmed_predictions = []
        for b in range(len(packed_predictions)):
            num_valid = valid_lengths[b].item()
            trimmed_predictions.append(packed_predictions[b][:num_valid])
        
        return trimmed_predictions
