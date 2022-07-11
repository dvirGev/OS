/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>
#include <pthread.h>

#define PORT "9034" // the port client will be connecting to 

#define MAXDATASIZE 100 // max number of bytes we can get at once 

int connected;
int sockfd, numbytes;
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }

    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
void *recvFunc(void *arg)
{
    char buff[1024] = {0};
    int bytes = 0;
    while ((bytes = recv(sockfd, buff, 1024, 0)) != -1)
    {
        if (!bytes)
        {
            connected = 0;
            break;
        }
        printf("\nGET> %s\n",buff);
        if (!strcmp(buff, "exit"))
        {
            connected = 0;
            break;
        }
        bzero(buff, 1024);
    }
    return NULL;
}
void *sendFunc(void *arg)
{
    char input[1024] = {0};
    while (connected != 0)
    {
        
        printf("SEND>");
        gets(input);
        // printf("\n",);
        if (strncmp(input,"exit",4) == 0)
        {
            send(sockfd,"exit",4,0);
            connected = 0;
            break;
        }
        if (send(sockfd, input, strlen(input) + 1, 0) == -1)
        {
            perror("send");
        }
        bzero(input, 1024);
    }
    return NULL;
}
int main(int argc, char *argv[])
{
      
    char buf[MAXDATASIZE];
    struct addrinfo hints, *servinfo, *p;
    int rv;
    char s[INET6_ADDRSTRLEN];

    if (argc != 2) {
        fprintf(stderr,"usage: client hostname\n");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
        return 1;
    }

    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("client: connect");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
            s, sizeof s);
    printf("client: connecting to %s\n", s);

    freeaddrinfo(servinfo); // all done with this structure

    // if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
    //     perror("recv");
    //     exit(1);
    // }

    pthread_t pair_threads[2];
    connected = 1;
    pthread_create(&pair_threads[0], NULL, recvFunc, NULL);
    pthread_create(&pair_threads[1], NULL, sendFunc, NULL);
    // pthread_join(pair_threads[0], NULL);
    pthread_join(pair_threads[0], NULL);
    pthread_join(pair_threads[0], NULL);
    pthread_kill(pair_threads[1], 0);
    pthread_kill(pair_threads[1], 0);
    close(sockfd);
    printf("The client send 'exit' and turn off\n");
    return 0;
}