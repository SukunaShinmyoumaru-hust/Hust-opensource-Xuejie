#coding=utf-8

from . import dict_generator
# import dict_generator
class MMSeg:
    """ 基于最大匹配法(Maximum Matching)的中文分词器. """
    def __init__(self):
        self.dict = None

    def set_dict(self, mdict):
        """ 设置词典. """
        self.dict = mdict

    def cut(self, sent, mode='bimm'):
        """ 切分给定字符串,返回切分结果.

            Args:
              sent: 待切分字符串.
              mode: 选择不同的切分算法,可选值有:'fmm','bmm','bimm'.

            Returns:
              切分结果,一个列表,包含切分出来的各个词语.
        """
        if(mode not in ['fmm', 'bmm', 'bimm']):
            raise ValueError("invalid value for mode, only accept 'fmm', 'bmm' and 'bimm'")

        if(mode == 'fmm'):
            return self.__fmm_cut(sent)
        elif(mode == 'bmm'):
            return self.__bmm_cut(sent)
        else:
           return self.__bimm_cut(sent)

    def __fmm_cut(self, sent):
        """ 使用fmm(正向最大匹配算法)切分句子
        """
        result = []

        i, j = 0, len(sent)
        while i <= len(sent)-1:
            while i+1 < j:
                if(sent[i:j] in self.dict.keys()):
                    break
                elif self.__is_date_or_number(sent[i:j]):
                    break
                elif self.__special_string_handle(sent[i:j]):
                    break
                else:
                    j -= 1
            result.append(sent[i:j])
            i = j
            j = len(sent)
        
        return result

    def __bmm_cut(self, sent):
        """ 使用bmm(反向最大匹配算法)切分句子
        """
        result = []
        self.max_len_in_dict = self.__biggest_len(self.dict)

        if(self.max_len_in_dict > len(sent)):
            max_len = len(sent)
        else:
            max_len = self.max_len_in_dict
        
        i, j = len(sent) - max_len, len(sent)
        while j > 0:
            while i+1 < j:
                if(sent[i:j] in self.dict.keys()):
                    break
                elif self.__is_date_or_number(sent[i:j]):
                    break
                elif self.__special_string_handle(sent[i:j]):
                    break
                else:
                    i += 1
            result.append(sent[i:j])
            j = i
            i = i - max_len

        result.reverse()

        return result

    def __bimm_cut(self, sent):
        """ 使用bi-mm(双向最大匹配算法)切分句子
        """
        fmm_list = self.__bmm_cut(sent)
        bmm_list = self.__fmm_cut(sent)

        if(len(fmm_list) != len(bmm_list)):
            if(len(fmm_list) < len(bmm_list)):
                return fmm_list
            else:
                return bmm_list
                
        else:
            FSingle = 0
            BSingle = 0
            for i in range(len(fmm_list)):
                if(len(fmm_list[i]) == 1):
                    FSingle += 1
                if(len(bmm_list[i]) == 1):
                    BSingle += 1
            if(fmm_list == bmm_list):
                return fmm_list
            else:
                if(BSingle > FSingle):
                    return fmm_list
                else:
                    return bmm_list

    def __is_date_or_number(self, string):
        '''判断一个字符串是否为数字或者日期
        '''
        length = len(string)
        if(length < 2):
            return 0
        #print(length)
        
        if(string.isdigit()):
            return 1
        elif((string[length-1]=='年' or string[length-1]=='月' or string[length-1]=='日'or string[length-1]=='时'
              or string[length-1]=='分') and string[0:length-2].isdigit()):
            return 1
        elif(length == 2 and(string[length-1]=='年' or string[length-1]=='月' or string[length-1]=='日'
                or string[length-1]=='时' or string[length-1]=='分') and string[0].isdigit()):
            return 1
        else:
            return 0

    def __judge_string(self, check_str):
        for ch in check_str:
            if('a' <= ch <= 'z' or 'A'<= ch <= 'Z' or '0'<= ch <='9' or ch =='％' or ch =='．'or ch == '@'or ch =='.'):
                continue
            else:
                return 0
                break
        return 1

    def __special_string_handle(self, string):
        '''判断一个字符串是否为特殊情况
        '''
        list1 = ['月','日','时','分']
        list2 = ['亿','万']
        length = len(string)
        if(length < 2):
            return 0
        #print(length)

        #不包含中文和℃的情况
        if(self.__judge_string(string) == 1):
            return 1
        #日期的情况 其中得派出并不是指时间的年 例如“过去了90年” 长度的判断 需要分开写
        elif(string[length-1] == '年' and length >= 5 and string[0:length-2].isdigit()):
            return 1
        elif(string[length-1] in list1 and string[0:length-2].isdigit()):
            return 1
        elif(length == 2 and string[length-1] in list1 and string[0].isdigit()):
            return 1
        #数量单位的处理“亿” “万” “万亿”
        elif(string[length-1] in list2 and self.__judge_string(string[0:length-2]) == 1):
            return 1
        elif(length >= 3 and string[length-2:length-3] == '万亿' and self.__judge(string[0:length-2]) == 1):
            return 1
        else:
            return 0

    def __biggest_len(self, corpus_dict):
        dict_list = list(corpus_dict.keys())
        max = 0
        for i in range(len(dict_list)):
            if max < len(dict_list[i]):
                max = len(dict_list[i])

        return max
        
if __name__ == '__main__':
    s = "本报南昌讯记者鄢卫华报道：１７日上午，由本报和圣象·康树联合主办的瓦尔德内尔挑战赛在南昌圆满落幕。"
    
    print("原始句子:" + s + "\n")

    # mdict = dict_generator.json_read("dicts/shanxi_dict.json", encoding='utf-16')
    mdict = dict_generator.load_sogou_dict('datasets/SogouLabDic.dic')

    seg = MMSeg()
    seg.set_dict(mdict)

    # FMM前向算法测试
    print("----- FMM前向算法分词结果 -----")
    for word in seg.cut(s, mode='fmm'):
        print(word,end="/")
    print()

    # BMM后向算法测试
    print("----- BMM后向算法分词结果 -----")
    for word in seg.cut(s, mode='bmm'):
        print(word,end="/")
    print()

    # MM双向算法测试
    print("----- MM双向算法分词结果 -----")
    for word in seg.cut(s, mode='bimm'):
        print(word,end="/")
    print()