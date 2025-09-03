#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void (*phase)(int  i);   // 定义了一个函数指针

int gencookie(char *s)
{
        if (strlen(s) !=10)  {
          printf("length of userid  must be 10. \n");
          return 0;
     }
     if (s[0] !='U' && s[0] !='u')  {
          printf("student id  satrt with U. \n");
          return 0;
     }
     for(int i=1;i<10;i++) 
         if (s[i]<'0' || s[i]>'9') {
          printf("stuid must be digitals. \n");
          return 0;
     }
     return 3+atoi(s+9);   
}

int main(int argc, const char *argv[])
{
     int     cookie;
     char  stuid[12];
     printf("please input you stuid : ");
     scanf("%s",stuid);
     cookie = gencookie(stuid);

     if (phase)
         (*phase)(cookie);
     else  {
          printf("Welcome to linkbomb \n");
          printf("You should modify phase1.o,  phase2.o ....\n");
          printf("execute :  gcc -no-pie -o linkbomb1 main.o phase1.o \n");
          printf("execute :  ./linkbomb1 \n");
    }
    printf("Bye Bye !\n");
    return 0; 
}
