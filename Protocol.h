/*
Carlos Daniel Estrada Guerra                                 A01039919
Erick Francisco González Martínez                            A01039859
Victor Andrés Villarreal                                    A01039863
*/
#define PROTOCOL_CREATE 1
#define PROTOCOL_MODIFY 2
#define PROTOCOL_DELETE 3

typedef struct {
    int type;
    int is_dir;
    char dir[256];
    char content[2048];
} Protocol;

extern const int MAX_PROTOCOL_SIZE; 