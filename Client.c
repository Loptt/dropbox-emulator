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
    // Verify the port entered (more than 3 numbers)
    if (argc < 3) {
    fprintf(stderr,"usage %s hostname port\n", argv[0]);
    exit(0);
    }

    signal(SIGINT, handle_sigint);

    start_monitor(argv[1], argv[2]);

    return 0;
}
