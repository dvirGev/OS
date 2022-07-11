// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
#ifndef test
#define test
#include "Ex5.h"
#endif

typedef struct STACK
{
    size_t index;
    char data[MAXDATASIZE * 1000];
} stack, *pstack;

void intitStack(pstack st)
{
    st->index = MAXDATASIZE * 1000;
}
void PUSH(pstack st, char *txt)
{
    lock.l_type = F_WRLCK;
    fcntl(fd, F_SETLKW, &lock);
    if (st->index - strlen(txt) < 0)
    {
        printf("error,max stack size");
        return;
    }
    st->index -= (strlen(txt) + 1);
    strcpy(&(st->data[st->index]), txt);
    lock.l_type = F_UNLCK;
    fcntl (fd, F_SETLKW, &lock);
}
int IsEmpty(pstack st)
{
    return (st->index == MAXDATASIZE * 1000);
}
char *TOP(pstack st)
{
    lock.l_type = F_WRLCK;
    fcntl(fd, F_SETLKW, &lock);
    if (IsEmpty(st))
    {
        perror("the stack is empty");
    }
    char *temp = (char *)malloc(sizeof(char) * MAXDATASIZE);
    strcpy(temp, &(st->data[st->index]));
    lock.l_type = F_UNLCK;
    fcntl (fd, F_SETLKW, &lock);
    return temp;
}
char *POP(pstack st)
{
    lock.l_type = F_WRLCK;
    fcntl(fd, F_SETLKW, &lock);
    if (IsEmpty(st))
    {
        perror("the stack is empty");
    }
    char *res = (char *)malloc(sizeof(char) * MAXDATASIZE);
    strcpy(res, &(st->data[st->index]));
    st->index += strlen(res) + 1;
    lock.l_type = F_UNLCK;
    fcntl (fd, F_SETLKW, &lock);
    return res;
}