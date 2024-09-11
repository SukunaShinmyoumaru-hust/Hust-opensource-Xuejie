#include "Sat_Puzzle.h"
int rule_1(char filename[80]) {
	FILE* fp;
	fp = fopen(filename, "a+");
	if (fp == NULL) {
		printf("打开文件失败\n");
		return 0;
	}
	/*
	fprintf(fp,"cccccc\n");
	fprintf(fp,"p cnf 36 2376\n");
	*/
	int i, j, k;                 //约束1，每一行，每一列，没有连续的三个0或者三个1 
	for (i = 1; i <= 6; i++) {             //每一行 考虑 
		for (j = 1; j <= 4; j++) {
			for (k = 0; k <= 2; k++) {
				fprintf(fp, "%d ", (i - 1) * 6 + j + k);
			}
			fprintf(fp, "0\n");
			for (k = 0; k <= 2; k++) {
				fprintf(fp, "%d ", -((i - 1) * 6 + j + k));
			}
			fprintf(fp, "0\n");
		}
	}
	for (j = 1; j <= 6; j++) {            //每一列考虑 
		for (i = 1; i <= 4; i++) {
			for (k = 0; k <= 2; k++) {
				for (k = 0; k <= 2; k++) {
					fprintf(fp, "%d ", (i - 1) * 6 + j + k * 6);
				}
				fprintf(fp, "0\n");
				for (k = 0; k <= 2; k++) {
					fprintf(fp, "%d ", -((i - 1) * 6 + j + k * 6));
				}
				fprintf(fp, "0\n");
			}
		}
	}
	fclose(fp);
	return OK;
} //此时，子句数为96个，变元为1~36 

int rule_2(char filename[80]) {      //约束2，每一行 ，每一列的0与1数目相等（=3），即任意4个单元，不全为0，不全为1 
	FILE* fp;
	fp = fopen(filename, "a+");
	if (fp == NULL) {
		printf("打开文件失败\n");
		return 0;
	}
	int i, j, k, w;
	int n;
	for (n = 0; n <= 5; n++) {       //考虑行        
		for (i = 1; i <= 3; i++) {
			for (j = 2; j <= 4; j++) {
				for (k = 3; k <= 5; k++) {
					for (w = 4; w <= 6; w++) {
						if (i < j && j < k && k < w) {
							fprintf(fp, "%d %d %d %d 0\n", i + 6 * n, j + 6 * n, k + 6 * n, w + 6 * n);
							fprintf(fp, "%d %d %d %d 0\n", -(i + 6 * n), -(j + 6 * n), -(k + 6 * n), -(w + 6 * n));
						}

					}
				}

			}
		}
	}
	for (n = 0; n <= 5; n++) {       //考虑列       
		for (i = 1; i <= 13; i = i + 6) {
			for (j = 7; j <= 19; j = j + 6) {
				for (k = 13; k <= 25; k = k + 6) {
					for (w = 19; w <= 31; w = w + 6) {
						if (i < j && j < k && k < w) {
							fprintf(fp, "%d %d %d %d 0\n", i + n, j + n, k + n, w + n);
							fprintf(fp, "%d %d %d %d 0\n", -(i + n), -(j + n), -(k + n), -(w + n));
						}

					}
				}

			}
		}
	}
	fclose(fp);
	return OK;
}

int rule_3(char filename[80]) {                //约束三，没有两行，两列相同 
	FILE* fp;
	fp = fopen(filename, "a+");
	if (fp == NULL) {
		printf("打开文件失败\n");
		return 0;
	}
	int i, j, k, w, r, s, t;
	for (i = 1; i <= 6; i++) {              //考虑行 
		for (k = 1; k <= 6 - i; k++) {		//只和该行后面的行的对应文字结对
			for (w = 0; w <= 6; w++) {      //标记有几个为负
				if (w == 0) {		//全为正
					for (j = 1; j <= 6; j++) {
						fprintf(fp, "%d %d ", (i - 1) * 6 + j, (i - 1) * 6 + j + 6 * k);
					}
					fprintf(fp, "0\n");
				}
				else if (w == 1) {            //一个为负 
					for (r = 1; r <= 6; r++) {       //标记当前哪几个文字为负：r
						for (j = 1; j <= 6; j++) {
							if (j == r) fprintf(fp, "%d %d ", -((i - 1) * 6 + j), -((i - 1) * 6 + j + 6 * k));
							else fprintf(fp, "%d %d ", (i - 1) * 6 + j, (i - 1) * 6 + j + 6 * k);
						}
						fprintf(fp, "0\n");
					}
				}
				else if (w == 2) {  //两个为负 
					for (r = 1; r <= 6; r++) {		//标记当前哪几个文字为负：r、s（r > s）
						for (s = r + 1; s <= 6; s++) {
							for (j = 1; j <= 6; j++) {
								if (j == r || j == s)  fprintf(fp, "%d %d ", -((i - 1) * 6 + j), -((i - 1) * 6 + j + 6 * k));
								else fprintf(fp, "%d %d ", (i - 1) * 6 + j, (i - 1) * 6 + j + 6 * k);
							}
							fprintf(fp, "0\n");
						}
					}
				}
				else if (w == 3) {  //三个为负 
					for (r = 1; r < 6; r++) {		//标记当前哪几个文字为负：r、s、t（r > s > t）
						for (s = r + 1; s <= 6; s++) {
							for (t = s + 1; t <= 6; t++) {
								for (j = 1; j <= 6; j++) {
									if (j == r || j == s || j == t) fprintf(fp, "%d %d ", -((i - 1) * 6 + j), -((i - 1) * 6 + j + 6 * k));
									else fprintf(fp, "%d %d ", (i - 1) * 6 + j, (i - 1) * 6 + j + 6 * k);
								}
								fprintf(fp, "0\n");
							}
						}
					}
				}
				else if (w == 4) {           //四个为负即两个为正 
					for (r = 1; r <= 6; r++) {
						for (s = r + 1; s <= 6; s++) {
							for (j = 1; j <= 6; j++) {
								if (j == r || j == s)  fprintf(fp, "%d %d ", (i - 1) * 6 + j, (i - 1) * 6 + j + 6 * k);
								else fprintf(fp, "%d %d ", -((i - 1) * 6 + j), -((i - 1) * 6 + j + 6 * k));
							}
							fprintf(fp, "0\n");
						}
					}
				}
				else if (w == 5) {			//一个为正
					for (r = 1; r <= 6; r++) {       //标记当前对哪个数为负
						for (j = 1; j <= 6; j++) {
							if (j == r) fprintf(fp, "%d %d ", (i - 1) * 6 + j, (i - 1) * 6 + j + 6 * k);
							else fprintf(fp, "%d %d ", -((i - 1) * 6 + j), -((i - 1) * 6 + j + 6 * k));
						}
						fprintf(fp, "0\n");
					}
				}
				else if (w == 6) {       //六个为负 
					for (j = 1; j <= 6; j++) {
						fprintf(fp, "%d %d ", -((i - 1) * 6 + j), -((i - 1) * 6 + j + 6 * k));
					}
					fprintf(fp, "0\n");
				}
			}

		}
	}

	for (i = 1; i <= 6; i++) {   //考虑列 
		for (k = i + 1; k <= 6; k++) {
			for (w = 0; w <= 6; w++) {
				if (w == 0) {
					for (j = 1; j <= 6; j++) {
						fprintf(fp, "%d %d ", i + (j - 1) * 6, k + (j - 1) * 6);
					}
					fprintf(fp, "0\n");
				}
				else if (w == 1) {
					for (r = 1; r <= 6; r++) {       //标记当前对哪个数为负
						for (j = 1; j <= 6; j++) {
							if (j == r) fprintf(fp, "%d %d ", -(i + (j - 1) * 6), -(k + (j - 1) * 6));
							else fprintf(fp, "%d %d ", i + (j - 1) * 6, k + (j - 1) * 6);
						}
						fprintf(fp, "0\n");
					}
				}
				else if (w == 2) {
					for (r = 1; r <= 6; r++) {
						for (s = r + 1; s <= 6; s++) {
							for (j = 1; j <= 6; j++) {
								if (j == r || j == s)  fprintf(fp, "%d %d ", -(i + (j - 1) * 6), -(k + (j - 1) * 6));
								else fprintf(fp, "%d %d ", i + (j - 1) * 6, k + (j - 1) * 6);
							}
							fprintf(fp, "0\n");
						}
					}
				}
				else if (w == 3) {  //三个为负 
					for (r = 1; r < 6; r++) {
						for (s = r + 1; s <= 6; s++) {
							for (t = s + 1; t <= 6; t++) {
								for (j = 1; j <= 6; j++) {
									if (j == r || j == s || j == t) fprintf(fp, "%d %d ", -(i + (j - 1) * 6), -(k + (j - 1) * 6));
									else fprintf(fp, "%d %d ", i + (j - 1) * 6, k + (j - 1) * 6);
								}
								fprintf(fp, "0\n");
							}
						}
					}
				}
				else if (w == 4) {           //四个为负 
					for (r = 1; r <= 6; r++) {
						for (s = r + 1; s <= 6; s++) {
							for (j = 1; j <= 6; j++) {
								if (j == r || j == s)  fprintf(fp, "%d %d ", i + (j - 1) * 6, k + (j - 1) * 6);
								else fprintf(fp, "%d %d ", -(i + (j - 1) * 6), -(k + (j - 1) * 6));
							}
							fprintf(fp, "0\n");
						}
					}
				}
				else if (w == 5) {
					for (r = 1; r <= 6; r++) {       //标记当前对哪个数为负
						for (j = 1; j <= 6; j++) {
							if (j == r) fprintf(fp, "%d %d ", i + (j - 1) * 6, k + (j - 1) * 6);
							else fprintf(fp, "%d %d ", -(i + (j - 1) * 6), -(k + (j - 1) * 6));
						}
						fprintf(fp, "0\n");
					}
				}
				else if (w == 6) {       //六个为负 
					for (j = 1; j <= 6; j++) {
						fprintf(fp, "%d %d ", -(i + (j - 1) * 6), -(k + (j - 1) * 6));
					}
					fprintf(fp, "0\n");
				}
			}
		}
	}
	fclose(fp);
	return OK;
}

