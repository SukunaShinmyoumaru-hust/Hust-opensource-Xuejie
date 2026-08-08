import torch
import torch.nn as nn
from torch.nn.utils.rnn import pack_padded_sequence, pad_packed_sequence

try:
    from torchcrf import CRF as _CRF

    def make_crf(num_tags):
        return _CRF(num_tags, batch_first=True)

    def crf_loss(crf, emissions, tags, mask):
        try:
            return -crf(emissions, tags, mask, reduction='mean')
        except TypeError:
            return -crf(emissions, tags, mask).mean()

    def crf_decode(crf, emissions, mask):
        if hasattr(crf, 'decode'):
            return crf.decode(emissions, mask)
        return crf.viterbi_decode(emissions, mask)
except ModuleNotFoundError:
    from TorchCRF import CRF as _CRF

    def make_crf(num_tags):
        return _CRF(num_tags)

    def crf_loss(crf, emissions, tags, mask):
        try:
            return -crf(emissions, tags, mask).mean()
        except TypeError:
            return -crf(emissions, tags, mask, reduction='mean')

    def crf_decode(crf, emissions, mask):
        if hasattr(crf, 'viterbi_decode'):
            return crf.viterbi_decode(emissions, mask)
        return crf.decode(emissions, mask)


class CWS(nn.Module):

    def __init__(self, vocab_size, tag2id, embedding_dim, hidden_dim):
        super(CWS, self).__init__()
        self.embedding_dim = embedding_dim
        self.hidden_dim = hidden_dim
        self.vocab_size = vocab_size
        self.tag2id = tag2id
        self.tagset_size = len(tag2id)

        self.word_embeds = nn.Embedding(vocab_size + 1, embedding_dim)

        self.lstm = nn.LSTM(embedding_dim, hidden_dim // 2, num_layers=1,
                            bidirectional=True, batch_first=True)
        self.hidden2tag = nn.Linear(hidden_dim, self.tagset_size)

        self.crf = make_crf(4)

    def init_hidden(self, batch_size, device):
        return (torch.randn(2, batch_size, self.hidden_dim // 2, device=device),
                torch.randn(2, batch_size, self.hidden_dim // 2, device=device))

    def _get_lstm_features(self, sentence, length):
        batch_size, seq_len = sentence.size(0), sentence.size(1)

        # idx->embedding
        embeds = self.word_embeds(sentence.view(-1)).reshape(batch_size, seq_len, -1)
        embeds = pack_padded_sequence(embeds, length, batch_first=True)

        # LSTM forward
        self.hidden = self.init_hidden(batch_size, sentence.device)
        lstm_out, self.hidden = self.lstm(embeds, self.hidden)
        lstm_out, _ = pad_packed_sequence(lstm_out, batch_first=True)
        lstm_feats = self.hidden2tag(lstm_out)
        return lstm_feats

    def forward(self, sentence, tags, mask, length):
        emissions = self._get_lstm_features(sentence, length)
        return crf_loss(self.crf, emissions, tags, mask)

    def infer(self, sentence, mask, length):
        emissions = self._get_lstm_features(sentence, length)
        return crf_decode(self.crf, emissions, mask)
