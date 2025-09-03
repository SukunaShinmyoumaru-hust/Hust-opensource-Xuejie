#include <stdio.h>
void do_phase(int);

void (*phase)(int pos)=do_phase;   // 定义了一个函数指针

void do_phase(int pos)
{
     printf("Hello. Do you understand strong / weak symbol ? \n");
     printf(" function pointer phase is a weak symbol in main.c \n");
     printf(" function pointer phase is a strong symbol in phase0.c \n");
     printf(" So, You can compare difference of two program : \n");
     printf("        gcc  -no-pie -o linkbomb0 main.c phase0.c \n");
     printf("        gcc  -no-pie -o linkbomb main.c \n");
     printf(" Are you ready to difuse linkbomb?  gcc -no-pie -o linkbomb* main.c phase*.o \n");
}
