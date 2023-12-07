import pandas as pd
# 读入邮件信息
emails = pd.read_csv('datasets/Emails.csv')
# 读入别名信息
aliases = {}
f = pd.read_csv('datasets/Aliases.csv')
for index, row in f.iterrows():
    aliases[row['Alias']] = row['PersonId']
# 读入人名信息
persons = {}
persons_ = {}
f = pd.read_csv('datasets/Persons.csv')
for index, row in f.iterrows():
    persons[row['Id']] = row['Name']
    persons_[row['Name']] = row['Id']

def name_uni(name):
    """
    对人名进行统一
    :param name: 人名（可能是别名）
    :return: 统一后的人名
    """
    name = str(name).lower()
    name = name.replace(',', '')
    # 别名转换
    if name in aliases.keys():
        return persons[aliases[name]]
    return name

def generate_egdes():
    """
    由邮件信息中的发送者和接收者构造有向图的边
    :return:
    """
    # 将寄件人和收件人的姓名格式化
    emails.MetadataFrom = emails.MetadataFrom.apply(name_uni)
    emails.MetadataTo = emails.MetadataTo.apply(name_uni)

    sent_receive = {}
    for index, row in emails.iterrows():
        # 去掉不符合要求的
        if row['MetadataFrom'] not in persons_.keys() or row['MetadataTo'] not in persons_.keys():
            continue
        sent_receive[index] = [persons_[row['MetadataFrom']], persons_[row['MetadataTo']]]
    # print(sent_receive)
    name = ['sent_id', 'receive_id']
    data = pd.DataFrame.from_dict(sent_receive, orient='index', columns=name)
    data.to_csv('sent_receive.csv')