#ifndef _CLIENT_
#define _CLIENT_


typedef struct clientTag{
    int id;
    char* name;
    struct clientTag *next;
} Client;


int recieved_handshake;

pthread_t chat_thread;


ENetHost *client;

Client *this_client;

int init_client(char* host);
void * chat_handler( void *ptr );
int client_event_handle();
int client_parse_packet(ENetEvent e);
int send_server_message();
int deinit_client();
int update_server();
int client_actions();
int initial_connect();
int disconnect();
#endif
