/*
Carlos Daniel Estrada Guerra                                 A01039919
Erick Francisco González Martínez                            A01039859
Victor Andrés Villarreal                                    A01039863
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "Protocol.h"

char *directory = "server_directory/";

void create_file(Protocol data)
{
    int path_length = strlen(directory) + strlen(data.dir);

    char *path = malloc(path_length);

    strcpy(path, directory);
    strcat(path, data.dir);

    if (data.is_dir)
    {
        mkdir(path, 0700);
    }
    else 
    {
        FILE *create;
        create = fopen(path, "w");
        
        if (!create) {
            printf("Error creating file/directory\n");
            exit(1);
        }

        fclose(create);
    }
    
    free(path);
}

void modify_file(Protocol data)
{    
    FILE *modify;

    int path_length = strlen(directory) + strlen(data.dir);

    char *path = malloc(path_length);

    strcpy(path, directory);
    strcat(path, data.dir);


    if (data.is_dir) 
    {
        return;
    }

    printf("MODIFYING with %s\n", data.content);

    modify = fopen(path, "w");

    if (!modify) {
        printf("Error opening file/directory\n");
        exit(1);
    }

    fprintf(modify,"%s\n",data.content);
    fclose(modify);

    free(path);
}

void delete_file(Protocol data)
{
    int path_length = strlen(directory) + strlen(data.dir);

    char *path = malloc(path_length);

    strcpy(path, directory);
    strcat(path, data.dir);

    if (remove(path) != 0) {
        printf("Unable to delete the file"); 
    }
    
    free(path);
}

void write_change(Protocol data)
{
    switch(data.type)
    {
        case PROTOCOL_CREATE:
            create_file(data);
            break;
        case PROTOCOL_MODIFY:
            modify_file(data);
            break;
        case PROTOCOL_DELETE:
            delete_file(data);
            break;
    }
}
