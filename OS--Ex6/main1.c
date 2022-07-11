#include "queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#define MAXDATASIZE 1024
#define LENGTH 10

int v = 0;
typedef void *(*pfunc)(void *str);
typedef struct active_object
{
    pqueue Q;
    void *(*func1)(void *);
    void *(*func2)(void *);
    pthread_t *pid;

} active_object, *p_active_object;

p_active_object ao1;
p_active_object ao2;
p_active_object ao3;
// pqueue q1;
// pqueue q2;
// pqueue q3;
typedef struct q_obj
{
    int fd;
    char word[MAXDATASIZE];
} q_obj, *pq_obj;

void newAo(p_active_object ao)
{

    while (1)
    {
        v++;
        printf("newao whilee---%d\n", v);
        // active func 1
        pq_obj obj2 = ao->func1(deQ(ao->Q));
        // active func 2
        printf("%s-newao\n", obj2->word);
        ao->func2(obj2);
    }
}
void destroyAO(p_active_object ao)
{
    destoryQ(ao->Q);
    free(ao->pid);
}
void *Caesar(pq_obj obj1)
{

    for (size_t i = 0; i < strlen(obj1->word); i++)
    {
        if (obj1->word[i] >= 97 && obj1->word[i] < 122)
        {
            obj1->word[i] += 1;
        }
        else if (obj1->word[i] == 122)
        {
            obj1->word[i] = 'a';
        }
        else if (obj1->word[i] >= 'A' && obj1->word[i] < 'Z')
        {
            obj1->word[i] += 1;
        }
        else if (obj1->word[i] == 'Z')
        {
            obj1->word[i] = 'A';
        }
    }
    printf("csare - %s\n", obj1->word);
    return obj1;
}
void *change_char(pq_obj obj1)
{

    for (size_t i = 0; i < strlen(obj1->word); i++)
    {
        if (obj1->word[i] >= 'a' && obj1->word[i] <= 'z')
        {
            obj1->word[i] -= 32;
        }
        else if (obj1->word[i] >= 'A' && obj1->word[i] <= 'Z')
        {
            obj1->word[i] += 32;
        }
    }
    return obj1;
}
void *send_answer(pq_obj obj1)
{
    printf("send?\n");
    if (send(obj1->fd, obj1->word, strlen(obj1->word), 0) == -1)
    {
        perror("send");
        exit(1);
    }

    return obj1;
}
void pipe1(pq_obj pq_obj1)
{
    enQ(ao2->Q, (void *)pq_obj1);
}
void pipe2(pq_obj pq_obj2)
{

    enQ(ao3->Q, pq_obj2);
}
void none(pq_obj pq_obj1)
{
    printf("was in none\n");
}

#define PORT "3490" // the port users will be connecting to

#define BACKLOG 10 // how many pending connections queue will hold

void sigchld_handler(int s)
{
    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while (waitpid(-1, NULL, WNOHANG) > 0)
        ;

    errno = saved_errno;
}
void *myThreadFun(int new_fd)
{
    printf("new client connect %d\n", new_fd);
    int numbytes;
    while (1)
    {
        char buf[MAXDATASIZE];
        if ((numbytes = recv(new_fd, buf, MAXDATASIZE - 1, 0)) == -1)
        {
            perror("recv");
            exit(1);
        }
        buf[numbytes] = '\0';
        printf("GET: %s\n", buf);
        if (!strncmp(buf, "EXIT", 4))
        {
            printf("Colse %d\n", new_fd);
            close(new_fd);
            break;
        }
        // קיבלתבת מידע
        // שלח את המידע
        // printf("SEND need to delete: %s\n", buf);
        // if (send(new_fd, buf, strlen(buf), 0) == -1)
        // {
        //     perror("send");
        //     exit(1);
        // }

        pq_obj q_obj1 = (pq_obj)malloc(sizeof(q_obj));
        strcpy(q_obj1->word, buf);
        q_obj1->fd = new_fd;

        enQ(ao1->Q, q_obj1);
        // work!
    }
    return NULL;
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in *)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6 *)sa)->sin6_addr);
}

int main(void)
{
    ao1 = (active_object *)malloc(sizeof(active_object));
    ao2 = (active_object *)malloc(sizeof(active_object));
    ao3 = (active_object *)malloc(sizeof(active_object));

    printf("heyyy1\n");
    ao1->func1 = &Caesar;
    printf("heyyy2\n");

    ao1->func2 = pipe1;

    ao1->Q = createQ();
    ao2->func1 = (pfunc)change_char;
    ao2->func2 = (pfunc)pipe2;
    ao2->Q = createQ();
    ao3->func1 = (pfunc)send_answer;
    ao3->Q = createQ();
    ao3->func2 = none;
    printf("heyyy\n");

    pthread_create(&(ao1->pid), NULL, newAo, ao1); //        pthread_create(&th, NULL, myThreadFun, new_fd);
    pthread_create(&(ao2->pid), NULL, newAo, ao2);
    pthread_create(&(ao3->pid), NULL, newAo, ao3);

    int sockfd, new_fd; // listen on sock_fd, new connection on new_fd
    struct addrinfo hints, *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes = 1;
    char s[INET6_ADDRSTRLEN];
    int rv;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE; // use my IP

    if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next)
    {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1)
        {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)
    {
        fprintf(stderr, "server: failed to bind\n");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    printf("server: waiting for connections...\n");

    while (1)
    { // main accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1)
        {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family,
                  get_in_addr((struct sockaddr *)&their_addr),
                  s, sizeof s);
        printf("server: got connection from %s\n", s);
        pthread_t th;
        pthread_create(&th, NULL, myThreadFun, new_fd);

        // if (i == threadSize)
        // {
        //     for (int j = 0; j < threadSize; j++)
        //     {
        //         pthread_join(th[j], NULL);
        //         printf("%d\n", j);
        //     }
        //     i = 0;
        // }
    }
    destroyAO(ao1);
    destroyAO(ao2);
    destroyAO(ao3);
    return 0;
}

// int main(int argc, char const *argv[])
// {
//     int arr[LENGTH] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
//     pqueue Q = createQ();
//     enQ(nex->Q)
//     for (size_t i = 0; i < LENGTH; i++)
//     {
//         enQ(Q, &arr[i]);
//     }
//     printf("[");
//     while (!isEmpty(Q))
//     {
//         int *n = (int*)deQ(Q);
//         printf("%d, ", *n);
//     }
//     printf("]\n");

//     destoryQ(Q);

//     return 0;
// }
