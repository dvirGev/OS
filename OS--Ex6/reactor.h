#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef void *(*pfunc)(void *arg);

typedef struct Reactor
{
    int fileID;
    pthread_t threadID;
    pfunc func;
} reactor, *preactor;

typedef struct reqests
{
    int fileID;
    preactor reac;
} reqests, *preqests;

preactor newReactor()
{
    preactor reac = (preactor)(malloc(sizeof(reactor)));
    return reac;
}
void InstallHandler(preactor reac, pfunc newFunc, int file_des)
{
    reac->func = newFunc;
    reac->fileID = file_des;
    preqests req = (preqests)(malloc(sizeof(reqests)));
    req->fileID = file_des;
    req->reac = reac;
    pthread_create(&reac->threadID, NULL, newFunc, req);
}
void RemoveHandler(preactor reac, int fd_free)
{
    pthread_join(reac->threadID, NULL);
    reac->fileID = -1;
    reac->func = NULL;
}