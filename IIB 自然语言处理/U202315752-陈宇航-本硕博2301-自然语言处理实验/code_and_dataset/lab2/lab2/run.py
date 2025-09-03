import pickle
import logging
import argparse
import os
import torch
from torch.utils.data import DataLoader
from torch.optim import Adam
from model import CWS
from dataloader import Sentence


def get_param():
    parser = argparse.ArgumentParser()
    parser.add_argument('--embedding_dim', type=int, default=100)
    parser.add_argument('--lr', type=float, default=0.005)
    parser.add_argument('--max_epoch', type=int, default=10)
    parser.add_argument('--batch_size', type=int, default=32)
    parser.add_argument('--hidden_dim', type=int, default=200)
    parser.add_argument('--cuda', action='store_true', default=False)
    return parser.parse_args()


def set_logger():
    log_file = os.path.join('save', 'log.txt')
    logging.basicConfig(
        format='%(asctime)s %(levelname)-8s %(message)s',
        level=logging.DEBUG,
        datefmt='%Y-%m%d %H:%M:%S',
        filename=log_file,
        filemode='w',
    )

    console = logging.StreamHandler()
    console.setLevel(logging.DEBUG)
    formatter = logging.Formatter('%(asctime)s %(levelname)-8s %(message)s')
    console.setFormatter(formatter)
    logging.getLogger('').addHandler(console)


def entity_split(x, y, id2tag, entities, cur):
    start, end = -1, -1
    for j in range(len(x)):
        if id2tag[y[j]] == 'B':
            start = cur + j
        elif id2tag[y[j]] == 'M' and start != -1:
            continue
        elif id2tag[y[j]] == 'E' and start != -1:
            end = cur + j
            entities.add((start, end))
            start, end = -1, -1
        elif id2tag[y[j]] == 'S':
            entities.add((cur + j, cur + j))
            start, end = -1, -1
        else:
            start, end = -1, -1


def main(args):
    use_cuda = args.cuda and torch.cuda.is_available()

    with open('data/ner_data_save.pkl', 'rb') as inp:
        word2id = pickle.load(inp)
        pickle.load(inp)
        tag2id = pickle.load(inp)
        pickle.load(inp)
        x_train = pickle.load(inp)
        y_train = pickle.load(inp)


    model = CWS(len(word2id), tag2id, args.embedding_dim, args.hidden_dim)
    if use_cuda:
        model = model.cuda()
    for name, param in model.named_parameters():
        logging.debug('%s: %s, require_grad=%s' % (name, str(param.shape), str(param.requires_grad)))

    optimizer = Adam(model.parameters(), lr=args.lr)

    train_data = DataLoader(
        dataset=Sentence(x_train, y_train),
        shuffle=True,
        batch_size=args.batch_size,
        collate_fn=Sentence.collate_fn,
        drop_last=False,
        num_workers=6
    )


    for epoch in range(args.max_epoch):
        step = 0
        log = []
        for sentence, label, mask, length in train_data:
            if use_cuda:
                sentence = sentence.cuda()
                label = label.cuda()
                mask = mask.cuda()

            # forward
            loss = model(sentence, label, mask, length)
            log.append(loss.item())

            # backward
            optimizer.zero_grad()
            loss.backward()
            optimizer.step()

            step += 1
            if step % 100 == 0:
                logging.debug('epoch %d-step %d loss: %f' % (epoch, step, sum(log) / len(log)))
                log = []

        path_name = "./save/model_epoch" + str(epoch) + ".pkl"
        torch.save(model, path_name)
        logging.info("model has been saved in  %s" % path_name)


if __name__ == '__main__':
    set_logger()
    main(get_param())
