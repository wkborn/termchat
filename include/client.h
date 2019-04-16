#ifndef _CLIENT_
#define _CLIENT_


typedef struct clientTag{
    int id;
    int enetid;
    char* name;
    struct clientTag *next;
} Client;


int recieved_handshake;



ENetHost *client;

Client *this_client;

char* to_send;

int init_client(char* host);
int chat_handler();
int client_event_handle();
int client_parse_packet(ENetEvent e);
int send_server_message();
int deinit_client();
int update_server();
int client_actions();
int initial_connect();
int disconnect();
int input_handle();
#endif
