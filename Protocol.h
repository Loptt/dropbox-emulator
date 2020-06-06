const int PROTOCOL_CREATE = 1;
const int PROTOCOL_MODIFY = 2;
const int PROTOCOL_DELETE = 3;

typedef struct {
    int type;
    char *dir;
    char *content;
} Protocol