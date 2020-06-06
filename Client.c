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
#include "Monitor.h"

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
    
    start_monitor(serv_addr, sockfd);
    
    return 0;
}
