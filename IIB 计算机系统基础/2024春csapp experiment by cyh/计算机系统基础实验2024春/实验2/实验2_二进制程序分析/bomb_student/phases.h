#ifndef __PHASES_H__
#define __PHASES_H__

extern void phase_1(char *input);
extern void phase_2(char *input);
extern void phase_3(char *input);
extern void phase_4(char *input);
extern void phase_5(char *input);
extern void phase_6(char *input);

typedef struct nodeStruct
{
    int value;
    int index;
    struct nodeStruct *next;
} listNode;

#endif /* __PHASES_H__ */
