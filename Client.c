/*
Carlos Daniel Estrada Guerra                                 A01039919
Erick Francisco González Martínez                            A01039859
Victor Andrés Villarreal                                    A01039863
*/
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>
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

    /* Our process ID and Session ID */
    pid_t pid, sid;
    
    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
            exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then
        we can exit the parent process. */
    if (pid > 0) {
            exit(EXIT_SUCCESS);
    }

    /* Change the file mode mask */
    umask(0);
            
    /* Open any logs here */        
            
    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
            /* Log the failure */
            exit(EXIT_FAILURE);
    }
    
    /* Close out the standard file descriptors */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
    
    /* Daemon-specific initialization goes here */
    
    /* The Big Loop */

    start_monitor(argv[1], argv[2]);

    exit(EXIT_SUCCESS);

}
