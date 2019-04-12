#ifndef _SERVER_
#define _SERVER_

ENetHost   *server;

Client *clients;
Message *messages;
int num_connected_clients;



int init_server();
int server_actions();
int server_event_handle();
int pack_packet();
int parse_packet(ENetEvent e);
int deinit_server();
int update_client();
int update_clients();
int count_clients(Client *c);
int remove_client(Client *c, int id);
int append_client(Client *c, char *name, int id);
Client *retrieve_client(Client *c, int id);
void print_clients(Client *c);
int destroy_client(Client *c);
int broadcast_message();
int broadcast_packet();
int packet_to_peer(int p);
#endif
