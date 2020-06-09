/*
Carlos Daniel Estrada Guerra                                 A01039919
Erick Francisco González Martínez                            A01039859
Victor Andrés Villarreal                                    A01039863
*/
void setup_connection(char *addr, char *portchar, struct sockaddr_in *serv_addr, int *sockfd);

void send_data(Protocol data, struct sockaddr_in serv_addr, int sockfd);