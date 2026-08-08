import torch
import pickle


def extract_entities(chars, tags, id2tag):
    """
    从字符序列和预测标签序列中提取有效实体
    :param chars: 字符列表
    :param tags: 预测的标签ID列表
    :param id2tag: 标签ID到标签字符串的映射
    :return: 实体列表 [(实体文本, 实体类别, 起始位置, 结束位置), ...]
    """
    entities = []
    entity_chars = []
    entity_type = None
    start_pos = -1

    for i, (ch, tag_id) in enumerate(zip(chars, tags)):
        tag = id2tag[tag_id]
        prefix = tag.split('-')[0] if '-' in tag else tag
        etype = tag.split('-')[1] if '-' in tag else None

        if prefix == 'B':
            # 如果之前有未闭合的实体，丢弃
            entity_chars = [ch]
            entity_type = etype
            start_pos = i
        elif prefix == 'I' and start_pos != -1 and etype == entity_type:
            entity_chars.append(ch)
        elif prefix == 'E' and start_pos != -1 and etype == entity_type:
            entity_chars.append(ch)
            entity_text = ''.join(entity_chars)
            entities.append((entity_text, entity_type, start_pos, i))
            entity_chars = []
            entity_type = None
            start_pos = -1
        elif prefix == 'S':
            entities.append((ch, etype, i, i))
            entity_chars = []
            entity_type = None
            start_pos = -1
        else:  # 'O' 或不匹配
            entity_chars = []
            entity_type = None
            start_pos = -1

    return entities


if __name__ == '__main__':
    model = torch.load('save/model.pkl', map_location=torch.device('cpu'), weights_only=False)
    model.eval()
    output = open('ner_result.txt', 'w', encoding='utf-8')

    with open('data/ner_datasave.pkl', 'rb') as inp:
        word2id = pickle.load(inp)
        id2word = pickle.load(inp)
        tag2id = pickle.load(inp)
        id2tag = pickle.load(inp)
        x_train = pickle.load(inp)
        y_train = pickle.load(inp)
        x_test = pickle.load(inp)
        y_test = pickle.load(inp)

    with open('data/ner_test.txt', 'r', encoding='utf-8') as f:
        line_chars = []
        for test in f:
            test = test.strip()

            if not test:
                if not line_chars:
                    continue

                # 构造模型输入
                vocab_size = model.word_embeds.num_embeddings
                x = torch.LongTensor(1, len(line_chars))
                mask = torch.ones_like(x, dtype=torch.uint8)
                length = [len(line_chars)]
                for i in range(len(line_chars)):
                    if line_chars[i] in word2id and word2id[line_chars[i]] < vocab_size - 1:
                        x[0, i] = word2id[line_chars[i]]
                    else:
                        x[0, i] = vocab_size - 1

                # 模型推断
                predict = model.infer(x, mask, length)[0]

                # 输出每个字的标签（保留原始逐字输出）
                for i in range(len(line_chars)):
                    print(line_chars[i], id2tag[predict[i]], file=output)
                print(file=output)

                # 提取并输出有效实体
                entities = extract_entities(line_chars, predict, id2tag)
                if entities:
                    sentence_text = ''.join(line_chars)
                    print(f"原句：{sentence_text}", file=output)
                    for ent_text, ent_type, start, end in entities:
                        print(f"  实体：{ent_text}\t类别：{ent_type}\t位置：{start}-{end}", file=output)
                    print(file=output)

                line_chars = []

            else:
                parts = test.split(' ')
                line_chars.append(parts[0])

    output.close()
    print("推断完成，结果保存至 ner_result.txt")
