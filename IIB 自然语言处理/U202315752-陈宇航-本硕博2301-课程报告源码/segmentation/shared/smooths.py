import numpy as np

def simple_good_turing2d(dict_matrix):

   '''
   采用simple_good_turing进行稀疏词典矩阵的平滑
   '''
   row_num = dict_matrix.shape[0]
   column_num = dict_matrix.shape[1]
   good_turing_matrix = np.zeros((row_num, column_num))
      
   i = 0
   while(i < row_num):
      fre_num_dict = {}
      new_list = []

      for p in range(column_num):
         if(dict_matrix[i][p] in fre_num_dict.keys()):
            fre_num_dict[dict_matrix[i][p]] += 1
         else:
            fre_num_dict[dict_matrix[i][p]] = 1

      temp_list = sorted(fre_num_dict.items(), key = lambda x:x[0], reverse = False)
      dict_len = len(fre_num_dict)

      #计算新的矩阵数据，需要的参数 n, N, N1, n1
      N = fre_num_dict[0]

      for p in range(dict_len):
         if(temp_list[p][0] <= 0):
            continue
         else:
            n1 = temp_list[p][0]
            N1 = fre_num_dict[n1]
            break
      if(abs(0 - n1) <= 2):
         c1 = n1 * N1 / N
      else:
         c1 = 1

      new_list = []   
      for j in range(column_num):
         n = dict_matrix[i][j]
         if(n == 0):
            new_list.append(c1)
         else:
            new_list.append(n + c1)
            '''if(c1 == 1):
               new_list.append(n + 1)
            else:
               new_list.append(n)'''
            
      new_list = new_list/sum(new_list)
      good_turing_matrix[i] = new_list
      i += 1

   return good_turing_matrix 

def simple_good_turing1d(dict_matrix):
   row_num = 1
   column_num = dict_matrix.shape[0]
   good_turing_matrix = np.zeros(column_num)

   fre_num_dict = {}
   new_list = []

   for p in range(column_num):
      if(dict_matrix[p] in fre_num_dict.keys()):
         fre_num_dict[dict_matrix[p]] += 1
      else:
         fre_num_dict[dict_matrix[p]] = 1

   temp_list = sorted(fre_num_dict.items(), key = lambda x:x[0], reverse = False)
   dict_len = len(fre_num_dict)

   #计算新的矩阵数据，需要的参数 n, N, N1, n1
   N = fre_num_dict[0]
   for p in range(dict_len):
      if(temp_list[p][0] <= 0):
         continue
      else:
         n1 = temp_list[p][0]
         N1 = fre_num_dict[n1]
         break
   if(abs(0 - n1) <= 2):
      c1 = n1 * N1 / N
   else:
      c1 = 1

   new_list = []   
   for j in range(column_num):
      n = dict_matrix[j]
      if(n == 0):
         new_list.append(c1)
      else:
         new_list.append(n + c1)
         
   new_list = new_list/sum(new_list)
   good_turing_matrix = new_list
   
   return good_turing_matrix
