#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define BUFFER 2048

int sockfd;

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

void handle_sigint(int sig)
{
    printf("\nStopping server...\n");
    close(sockfd);
    exit(0);
}

void server_loop() 
{
    int n, newsockfd;
    socklen_t clilen;
    char buffer[BUFFER];

    struct sockaddr_in cli_addr;

    while (1) 
    {
        listen(sockfd,5);
        clilen = sizeof(cli_addr); 

        // Create a connection with the found address
        newsockfd = accept(sockfd, 
                    (struct sockaddr *) &cli_addr, 
                    &clilen);

        // Check if there is an error accepting
        if (newsockfd < 0) {
            error("ERROR on accept");
        }

        bzero(buffer,BUFFER);

        // Read the message of the client into the buffer
        n = read(newsockfd,buffer,BUFFER-1);

        if (n < 0) {
            error("ERROR reading from socket");
        }

        printf("Received request: %s\n",buffer);

        n = write(newsockfd,"Success", 7);

        // Check if response was sent correctly
        if (n < 0) {
            error("ERROR writing to socket");
        }

        close(newsockfd);
    }
}

int main(int argc, char *argv[])
{
    const char *directory = "./server_directory/";

    int portno;
    struct sockaddr_in serv_addr;

    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    signal(SIGINT, handle_sigint);

    // Create new socket of type TCP over IP
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) {
        error("ERROR opening socket");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    // Get the port number from the command line argument
    portno = atoi(argv[1]);

    // Configure the server address of type IP and set the port number
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    // Bind the created socket to the defined server address and check for 
    // an error
    if (bind(sockfd, (struct sockaddr *) &serv_addr,
            sizeof(serv_addr)) < 0) {
                error("ERROR on binding");
            }


    server_loop(sockfd);

    // Close server and client sockets
    close(sockfd);
    return 0; 
}