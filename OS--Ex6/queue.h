#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>



typedef struct NODE
{
    void *data;
    struct NODE *next;
    struct NODE *prev;
} node, *pnode;

typedef struct QUEUE
{
    pnode start;
    pnode end;
    pthread_mutex_t lock;
    pthread_cond_t mutex_wait;
} queue, *pqueue;

pnode newNode(void *newData)
{
    pnode nod = (pnode)malloc(sizeof(node));
    if (nod == NULL)
    {
        perror("malloc pnode");
    }
    nod->data = newData;
    nod->next = NULL;
    nod->prev = NULL;
    return nod;
}

int isEmpty(pqueue Q)
{
    return (Q->start == NULL);
}
pqueue createQ()
{
    pqueue newQ = (pqueue)malloc(sizeof(queue));
    if (newQ == NULL)
    {
        perror("malloc pqueue");
        exit(0);
    }
    if (pthread_mutex_init(&(newQ->lock), NULL) != 0)
    {
        perror("mutex init failed");
        exit(0);
    }
    if (pthread_cond_init(&(newQ->mutex_wait), NULL) != 0)
    {
        perror("cond_init failed");
        exit(0);
    }
    newQ->start = NULL;
    newQ->end = NULL;
    
    
    return newQ;
}
void enQ(pqueue Q, void *n)
{   
    printf("-in enq\n");
    pthread_mutex_lock(&(Q->lock));
    pnode newN = newNode(n);
    if (isEmpty(Q))
    {
        Q->end = newN;
    }
    else
    {
        Q->start->prev = newN;
    }
    newN->next = Q->start;
    Q->start = newN;
    pthread_cond_signal(&(Q->mutex_wait));
    pthread_mutex_unlock(&(Q->lock));
    printf("-out enq\n");

}
void *deQ(pqueue Q)
{   
    printf("in deq\n");
    
    if (isEmpty(Q))
    {
        perror("The queue is empty!,we use cond when waiting\n");
        pthread_cond_wait(&(Q->mutex_wait), &(Q->lock));
        pthread_mutex_unlock(&(Q->lock));
        printf("after cond\n");
    }
    pthread_mutex_lock(&(Q->lock));
    printf("after the lock\n");
    if (Q->start == Q->end)
    {
        pnode temp = Q->start;
        Q->start = NULL;
        Q->end = NULL;
        void *res = temp->data;
        free(temp);
        return res;
    }
    pnode endN = Q->end;
    void *res = endN->data;
    endN->prev->next = NULL;
    Q->end = endN->prev;
    free(endN);
    pthread_mutex_unlock(&(Q->lock));
    return res;
}
void destoryQ(pqueue Q)
{
    while (!isEmpty(Q))
    {
        void *temp = deQ(Q);
        free(temp);
    }
    free(Q);
    pthread_cond_destroy(&(Q->mutex_wait));
    pthread_mutex_destroy(&(Q->lock));
}
