#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "Protocol.h"
#include "Sender.h"

#define PATH "./client_directory/"
#define MAX_EVENT_MONITOR 2048
#define NAME_LEN 256 //file name length
#define MONITOR_EVENT_SIZE (sizeof(struct inotify_event)) //size of one event
#define BUFFER_LEN MAX_EVENT_MONITOR*(MONITOR_EVENT_SIZE+NAME_LEN) //buffer length

int sockfd;

void error(const char *msg)
{
    perror(msg);
    exit(0);
}


int main(int argc, char *argv[])
{
    // Verify the port entered (more than 3 numbers)
    if (argc < 3) {
    fprintf(stderr,"usage %s hostname port\n", argv[0]);
    exit(0);
    }

    /* Our process ID and Session ID */
    pid_t pid, sid;

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0)
    {
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then
           we can exit the parent process. */
    if (pid > 0)
    {
        exit(EXIT_SUCCESS);
    }

    /* Change the file mode mask */
    umask(0);

    /* Open any logs here */

    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0)
    {
        /* Log the failure */
        exit(EXIT_FAILURE);
    }

    /* Close out the standard file descriptors */
    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    /* Daemon-specific initialization goes here */
    int fd,watch_desc;
    char buffer[BUFFER_LEN];
    fd=inotify_init();

    int sockfd;
    struct sockaddr_in serv_addr;

    char dataread[2048];
    long filelen;

    Protocol data;

    if(fd<0){ 
        perror("Notify not initialize");
    }

    watch_desc=inotify_add_watch(fd,PATH, IN_CREATE | IN_MODIFY | IN_DELETE);

    if(watch_desc==-1){
        printf("Could not add watch to %s \n",PATH);
    }
    else{
        printf("Monitor added to path: %s \n",PATH);
    }

    int i = 0;
    /* The Big Loop */
    while (1)
    {
        i = 0;
        int total_read = read(fd,buffer,BUFFER_LEN);

        if(total_read<0){
            perror("read error");
        }

        while(i < total_read) {
            struct inotify_event *event=(struct inotify_event*)&buffer[i];

            memset(&data, 0, sizeof(Protocol));

            if(event->len){
                if(event->mask & IN_CREATE){
                    data.type = PROTOCOL_CREATE;
                    strcpy(data.dir, event->name);
                    if(event->mask &IN_ISDIR){
                        data.is_dir = 1;
                        printf("Directory \"%s\"was created\n", event->name);
                    }
                    else{
                        data.is_dir = 0;
                        printf("File \"%s\"was created\n",event->name);
                    } 
                }
                if(event->mask & IN_MODIFY){
                    data.type = PROTOCOL_MODIFY;
                    strcpy(data.dir, event->name);
                    if(event->mask &IN_ISDIR){
                        data.is_dir = 1;
                        printf("Directory \"%s\"was modified\n", event->name);
                    }
                    else{
                        data.is_dir = 0;
                        int path_length = event->len + strlen(PATH);
                        char *path;

                        path = malloc(path_length);

                        strcpy(path, PATH);
                        strcat(path, event->name);

                        FILE *fp = fopen(path, "rb");


                        if (!fp) {
                            printf("Error opening file for modify: %s\n", path);
                            i += MONITOR_EVENT_SIZE+event->len;
                            break;
                        }

                        free(path);

                        fseek(fp, 0, SEEK_END);
                        filelen = ftell(fp);
                        rewind(fp);
                        
                        if (filelen < 2048)
                        {
                            fread(dataread, filelen, 1, fp);
                            dataread[filelen-1] = '\0';
                        }
                        else
                        {
                            fread(dataread, 2048, 1, fp);
                            dataread[2047-1] = '\0';
                        }
                        
                        fclose(fp);
                        strcpy(data.content, dataread);
                        printf("File \"%s\"was modified\n",event->name);
                    } 
                }
                if(event->mask & IN_DELETE){
                    data.type = PROTOCOL_DELETE;
                    strcpy(data.dir, event->name);
                    if(event->mask &IN_ISDIR){
                        data.is_dir = 1;
                        printf("Directory \"%s\"was deleted\n", event->name);
                    }
                    else{
                        data.is_dir = 0;
                        printf("File \"%s\"was deleted\n",event->name);
                    } 
                }
                i += MONITOR_EVENT_SIZE+event->len;
                setup_connection(argv[1], argv[2], &serv_addr, &sockfd);
                send_data(data, serv_addr, sockfd);
            } else {
                break;
            }
        sleep(10); /* wait 10 milliseconds */
    }
    inotify_rm_watch(fd,watch_desc);
    close(fd);
    exit(EXIT_SUCCESS);
}