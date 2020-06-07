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

void setup_connection(char *addr, char *portchar, struct sockaddr_in *serv_addr, int *sockfd)
{
     struct hostent *server;
     int portNo;

     portNo = atoi(portchar);
     *sockfd = socket(AF_INET, SOCK_STREAM, 0);


     if (*sockfd < 0) 
          perror("ERROR opening socket");

     server = gethostbyname(addr);
     //Error message if the client couldn't find the host
     if (server == NULL) {
          fprintf(stderr,"ERROR, no such host\n");
          exit(0);
     }

     bzero((char *) serv_addr, sizeof(*serv_addr));
     serv_addr->sin_family = AF_INET;
     bcopy((char *)server->h_addr, 
          (char *)&serv_addr->sin_addr.s_addr,
          server->h_length);
     serv_addr->sin_port = htons(portNo);
}

void send_data(Protocol data, struct sockaddr_in serv_addr, int sockfd) 
{
    int n;
    char *serialized;
    char buffer[256];

    serialized = (char *) &data;

     if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        perror("ERROR connecting");

     printf("SENDING ");

     for (int i = 0; i < strlen(data.content); i++)
     {
          printf("%02X ", data.content[i]);
     }

     printf("\n");

    n = write(sockfd, serialized, MAX_PROTOCOL_SIZE);

    if (n < 0) 
         perror("ERROR writing to socket");

    bzero(buffer, 256);
    n = read(sockfd,buffer, 255);

    if (n < 0) 
         perror("ERROR reading from socket");

    printf("%s\n",buffer);

     close(sockfd);
    
}