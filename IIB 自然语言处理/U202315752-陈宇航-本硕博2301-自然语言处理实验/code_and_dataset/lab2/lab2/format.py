with open('ner_result.txt', 'r', encoding='utf-8') as input_file, open('ner_result_format.txt', 'w', encoding='utf-8') as output_file:
    # 逐行读取输入文件
    for line in input_file:
        # 检查该行是否为空或开头是空白符
        if not line.strip():
            continue
        
        # 如果该行不为空且开头不是空白符，则将其写入输出文件
        output_file.write(line)

print("已去除开头是空白符的行。\n")
