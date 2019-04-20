#ifndef _CLIENT_
#define _CLIENT_



typedef struct clientTag{
    int id;
    ENetPeer *peer;
    char* name;
    struct clientTag *next;
} Client;

#include "message.h"

int recieved_handshake;

MessageSave *msg_stack;
int msg_index;

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
int print_messages(MessageSave *e, int index);
#endif
