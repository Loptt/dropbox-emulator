#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "Protocol.h"

void send_data(Protocol data, struct sockaddr_in serv_addr, int sockfd) 
{
    int n;
    char *serialized;
    char buffer[256];

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");


    serialized = (char *) &data;

    n = write(sockfd, serialized, MAX_PROTOCOL_SIZE);

    if (n < 0) 
         error("ERROR writing to socket");

    bzero(buffer, 256);
    n = read(sockfd,buffer, 255);

    if (n < 0) 
         error("ERROR reading from socket");

    printf("%s\n",buffer);

    close(sockfd);
}