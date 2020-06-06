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

int sockfd;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

void handle_sigint(int sig)
{
    printf("\nStopping client...\n");
    close(sockfd);
    exit(0);
}

void send_data(struct sockaddr_in serv_addr) 
{
    int n;
    char *buffer;
    Protocol data;

    data.type = PROTOCOL_CREATE;
    data.is_dir = 0;
    data.dir = "quetal.txt";
    data.content = "Hola que tal";

    buffer = malloc(MAX_PROTOCOL_SIZE);

    bzero(buffer, MAX_PROTOCOL_SIZE);

    buffer = (char *) &data;

    n = write(sockfd, buffer, MAX_PROTOCOL_SIZE);

    if (n < 0) 
         error("ERROR writing to socket");

    bzero(buffer,256);
    n = read(sockfd,buffer,255);

    if (n < 0) 
         error("ERROR reading from socket");

    printf("%s\n",buffer);

    free(buffer);
}

int main(int argc, char *argv[])
{
    int portNo;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    // Verify the port entered (more than 3 numbers)
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    signal(SIGINT, handle_sigint);

    portNo = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    //Error message if the client couldn't find the host
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portNo);

    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");


    send_data(serv_addr);

    close(sockfd);
    return 0;
}
