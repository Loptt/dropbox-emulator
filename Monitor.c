/*
Carlos Daniel Estrada Guerra                                 A01039919
Erick Francisco González Martínez                            A01039859
Victor Andrés Villarreal                                    A01039863
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <string.h>
#include <unistd.h>
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

void start_monitor(char *addr, char *port) {
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

    while(1) {
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
                setup_connection(addr, port, &serv_addr, &sockfd);
                send_data(data, serv_addr, sockfd);
            } else {
                break;
            }
        }
    }
    inotify_rm_watch(fd,watch_desc);
    close(fd);
}
