#include "Sat_Puzzle.h"
int deleteclau(pcnf L, int flag) {            //一次找出一个变元，并删除 
	pcNode p;    //子句头结点 
	plNode q;    //文字节点
	piNode r;    //索引表节点
	if (flag > 0) {//正文字
		r = L->Index_List[flag].firstpos;		// 先删去子句 
		while (r) {
			if (r->p_cnode->exist == 0)   r = r->next;//已经删掉的跳过去
			else {
				r->p_cnode->exist = 0;
				r->p_cnode->flag = flag;//该句因为包含flag这个关键文字被删除
				L->clau_num--;
				r = r->next;
			}
		}
		r = L->Index_List[flag].firstneg;		//删除文字-flag
		while (r) {
			if (r->p_cnode->exist == 0)  r = r->next;
			else {
				q = r->p_cnode->firstl;
				while (q) {
					if (q->exist == 0)  q = q->next;
					else {
						if (q->l == -flag) {
							q->exist = 0;
							r->p_cnode->l_num--;
							break;
						}
						q = q->next;
					}
				}
				r = r->next;
			}
		}
	}
	else {//负文字
		r = L->Index_List[-flag].firstneg;////// 先删去子句 
		while (r) {
			if (r->p_cnode->exist == 0)   r = r->next;
			else {
				r->p_cnode->exist = 0;
				r->p_cnode->flag = flag;
				L->clau_num--;
				r = r->next;
			}
		}
		r = L->Index_List[-flag].firstpos; //删除文字
		while (r) {
			if (r->p_cnode->exist == 0)  r = r->next;
			else {
				q = r->p_cnode->firstl;
				while (q) {
					if (q->exist == 0)  q = q->next;
					else {
						if (q->l == -flag) {
							q->exist = 0;
							r->p_cnode->l_num--;
							break;
						}
						q = q->next;
					}
				}
				r = r->next;
			}
		}
	}


	L->vari_num--;
	return OK;
}

int restoreclau(pcnf L, int flag) {       //恢复前面因为flag而删除的元素 
	pcNode p;    //子句头结点 
	plNode q;    //文字节点
	piNode r;    //索引表节点 
	if (flag > 0) {
		r = L->Index_List[flag].firstpos;          //添加子句
		while (r) {
			if (r->p_cnode->exist == 1)  r = r->next;
			else {
				if (r->p_cnode->flag == flag) {
					r->p_cnode->exist = 1;
					r->p_cnode->flag = 0;
					L->clau_num++;
				}
				r = r->next;
			}
		}
		r = L->Index_List[flag].firstneg;      //添加文字-flag
		while (r) {
			q = r->p_cnode->firstl;
			while (q) {
				if (q->exist == 0 && q->l == -flag) {
					q->exist = 1;
					r->p_cnode->l_num++;
					break;
				}
				q = q->next;
			}
			r = r->next;
		}
	}
	else {
		r = L->Index_List[-flag].firstneg;          //添加子句
		while (r) {
			if (r->p_cnode->exist == 1)  r = r->next;
			else {
				if (r->p_cnode->flag == flag) {
					r->p_cnode->exist = 1;
					r->p_cnode->flag = 0;
					L->clau_num++;
				}
				r = r->next;
			}
		}
		r = L->Index_List[-flag].firstpos;      //添加文字
		while (r) {
			q = r->p_cnode->firstl;
			while (q) {
				if (q->exist == 0 && q->l == -flag) {
					q->exist = 1;
					r->p_cnode->l_num++;
					break;
				}
				q = q->next;
			}
			r = r->next;
		}
	}
	L->vari_num++;             //变元数目加一 
	return OK;
}

int EmptyClause(pcnf L) {        //查看是否有空句 
	pcNode p = L->firstclau;
	while (p) {
		if (p->l_num == 0 && p->exist == 1) return OK;
		p = p->next;
	}
	return FALSE;
}

int Findl(pcnf L, AnsList& An) {
	pcNode p;
	plNode q;
	int i;
	int flag = 0;
	p = L->firstclau;
	while (p) {
		if (p->exist == 1 && p->l_num == 1) {      //寻找单子句 
			q = p->firstl;
			while (q) {
				if (q->exist == 1) {
					if (q->l > 0) 	An.elem[q->l] = 1;
					else    An.elem[-(q->l)] = -1;
					return q->l;
				}
				else q = q->next;
			}
		}
		else p = p->next;
	}
	//单子句不存在，就寻找出现次数最多的那个变元
	int* a = (int*)malloc((2 * An.length - 1) * sizeof(int));     //构建数组a，存储各变元出现的次数
	for (i = 0; i < 2 * An.length - 1; i++) {
		a[i] = 0;
	}
	p = L->firstclau;
	while (p) {                //记录各变元出现的次数 
		if (p->exist == 0) p = p->next;	//该子句被删除，跳过
		else {
			q = p->firstl;
			while (q) {
				if (q->exist == 0) q = q->next;
				else {
					if (q->l > 0) a[2 * (q->l) - 1]++;//正负文字挨着存次数
					else if (q->l < 0)    a[2 * (-(q->l))]++;
					q = q->next;
				}
			}
			p = p->next;
		}
	}
	for (i = 1; i < 2 * An.length - 1; i++) {    //找出出现次数的最大值 
		if (a[i] > flag)  flag = a[i];
	}
	for (i = 1; i < 2 * An.length - 1; i++) {  //找到变元 
		if (a[i] == flag)  break;
	}
	free(a);
	if (i % 2) {//i奇是正
		An.elem[(i + 1) / 2] = 1;
		return ((i + 1) / 2);
	}
	else {
		An.elem[i / 2] = -1;
		return (-(i / 2));
	}
}


int DPLL(pcnf L, AnsList& Ans, int pre_l) {
	int next_l;
	if (L->clau_num == 0) return OK;       //cnf公式为空，化简成功 
	else {
		if (EmptyClause(L) == OK) {       //有空子句 
			if (restoreclau(L, pre_l) != OK)  printf("恢复失败\n");
			if (pre_l > 0) Ans.elem[pre_l] = 0;
			else Ans.elem[-pre_l] = 0;
			return FALSE;
		}
		else {
			next_l = Findl(L, Ans);
			//printf("1"); 
			if (next_l == 0) return FALSE;//核心关键字为0，则不可满足

			if (deleteclau(L, next_l) != OK)  printf("删除失败\n");

			if (DPLL(L, Ans, next_l) == OK)  return OK;
			else {
				if (restoreclau(L, next_l) != OK) printf("恢复失败\n");
				if (deleteclau(L, -next_l) != OK) printf("删除失败\n");
				if (DPLL(L, Ans, -next_l) == OK) {
					if (next_l > 0) Ans.elem[next_l] = -1;//更新答案
					else  Ans.elem[-next_l] = 1;
					return OK;
				}
				else {
					if (restoreclau(L, -next_l) != OK) printf("恢复失败\n");
					if (next_l > 0) Ans.elem[next_l] = 0;
					else Ans.elem[-next_l] = 0;
					return FALSE;
				}
			}
		}
	}
}

int verify(pcnf L, AnsList& An) {       //检验答案是否正确 
	int flag;
	pcNode p;
	plNode q;
	p = L->firstclau;
	while (p) {
		flag = 0;
		q = p->firstl;
		while (q) {
			//存在一个文字为真
			if (((q->l > 0) && (An.elem[q->l] == 1)) || ((q->l < 0) && (An.elem[-(q->l)] == -1))) {
				flag = 1;
				break;
			}
			else q = q->next;
		}
		if (flag == 0) break;//所有文字为假
		p = p->next;
	}
	if (flag == 0)  return FALSE;
	else return TRUE;
}
