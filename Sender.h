void setup_connection(char *addr, char *portchar, struct sockaddr_in *serv_addr, int *sockfd);

void send_data(Protocol data, struct sockaddr_in serv_addr, int sockfd);