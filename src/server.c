#include "globals.h"

int init_server(){

  address = NULL;
  address = malloc(sizeof(ENetAddress));
  memset(address,0,sizeof(ENetAddress));

  if(enet_initialize() != 0){
    printw("Could not initialize networking.\n");
    return -1;
  }

  address->host = ENET_HOST_ANY;
  address->port = PORT;

  server = enet_host_create(address, MAX_CLIENTS, 2, 0, 0);
  if(server == NULL){
    free(server);
    server = NULL;
    return -1;
  }
  
    clients = NULL;

    printw("server created\n");
  return 0;
}

int server_actions(){

    //printw("running\n");

    server_event_handle();

    //sleep(1);
    //update_clients();
    //TODO server actions

    return 0;
}

int server_event_handle(){
  while(enet_host_service(server, &event, 0) > 0){
    switch(event.type){
      case ENET_EVENT_TYPE_CONNECT:
        printw("connection event\n");
      break;
      case ENET_EVENT_TYPE_RECEIVE:
        if(event.peer->data == NULL){
          event.peer->data = malloc(strlen((char*) event.packet->data)+1);
          memset(event.peer->data,0,strlen((char*) event.packet->data)+1);
          strcpy((char*) event.peer->data, (char*) event.packet->data);

          sprintf(net_buffer, "%s has connected.\n", (char*)event.peer->data);
          packet = enet_packet_create(net_buffer, strlen(net_buffer)+1, 0);
          enet_host_broadcast(server, 1, packet);
          enet_host_flush(server);
        } else {
          parse_packet(event);
        }
        break;
      case ENET_EVENT_TYPE_DISCONNECT:
        packet = enet_packet_create(net_buffer, strlen(net_buffer)+1, 0);
        enet_host_broadcast(server, 1, packet);
        enet_host_flush(server);
        break;
      default:
        break;
    }
  }
  return 0;
}

int parse_packet(ENetEvent e){
  char* packet_data = (char*)e.packet->data;
  sprintf(net_buffer, "%c", packet_data[0]);
  int opcode = atoi(net_buffer);
  switch(opcode){
    case CONNECT:
        
      if(count_clients(clients) >= MAX_CLIENTS){
        sprintf(net_buffer, "Server is full!");
        packet = enet_packet_create(net_buffer, strlen(net_buffer)+1, 0);
        enet_peer_send(event.peer, 0, packet);
        enet_host_flush(server);
      } else {
        char *saveptr;
        int id,enetid;
        char* name;
        // Now parse and assign data fields
        strtok_r((char*)e.packet->data ,":", &saveptr); // JUST THE OPCODE, can be thrown away
        id = atoi(strtok_r(NULL, ":", &saveptr));
        enetid=e.peer->connectID;
        name = strtok_r(NULL, ":", &saveptr);
        append_client(clients,name,id,enetid);
        printw("%s has joined the chatroom\n",name);
        snprintf(net_buffer,NETBUFFER_SIZE,"%d:%s:", SERVER_NOTICE, "Welcome to the server!");
        broadcast_packet_peer();
      }
    break;
    case ACTION:
      // For now it is just position data
    break;
    case MESSAGE:;
            // Print the message
            message_parse(e);
            broadcast_message();
    break;
    case DISCONNECT:;
        char *saveptr;
        int id;
        // Now parse and assign data fields
        strtok_r((char*)e.packet->data ,":", &saveptr); // JUST THE OPCODE, can be thrown away
        id = atoi(strtok_r(NULL, ":", &saveptr));
        Client *this = retrieve_client(clients,id);
        if(this==NULL){
            printw("illegal client\n");
            return -1;
        }
        printw("%s has disconnected\n",this->name);
        
    break;
  }

  return 0;
}

int broadcast_message(){
    snprintf(net_buffer,NETBUFFER_SIZE,"%d:%s:%s:",MESSAGE,messages->client->name,messages->msg);
    broadcast_packet();
    return 0;
}

int broadcast_packet(){
  int i;

  for(i = 0; i < server->peerCount; i++){
    packet = enet_packet_create(net_buffer, strlen(net_buffer)+1, 0);
    enet_peer_send(&server->peers[i], 0, packet);
    enet_host_flush(server);
  }

  return 0;
}

int broadcast_packet_peer(int enetid){
  int i;

  for(i = 0; i < server->peerCount; i++){
    if(&server->peers[i].connectID == enetid){
      packet = enet_packet_create(net_buffer, strlen(net_buffer)+1, 0);
      enet_peer_send(&server->peers[i], 0, packet);
      enet_host_flush(server);
    }
  }

  return 0;
}

int deinit_server(){
  enet_host_destroy(server);
  server=NULL;
  enet_deinitialize();
  glob_online=0;
  return 0;
}


int update_clients(){
  broadcast_packet();
  broadcast_packet();

  return 0;
}


int count_clients(Client *c){
    int count=0;
    while(c!=NULL){
        count++;
        c=c->next;
    }
    return count;
}

int append_client(Client *c, char *name, int id, int enetid){
    if(c == NULL){
        clients = calloc(1,sizeof(Client));
        clients->id = id;
        clients->enetid=enetid;
        clients->name= calloc(BUFFER_SIZE,sizeof(char));
        snprintf(clients->name,BUFFER_SIZE,"%s",name);
        return 0;
    }
    while(c!=NULL){
        if(c->next==NULL){
            c->next = calloc(1,sizeof(Client));
            c->next->id = id;
            c->next->name= calloc(BUFFER_SIZE,sizeof(char));
            snprintf(c->next->name,BUFFER_SIZE,"%s",name);
            c->next->next=NULL;
            return 0;
        }
        c=c->next;
    }
    return -1;
}

int remove_client(Client *c, int id){
    Client *prev = NULL;
    while(c!=NULL){
        if(c->id==id){
            if(prev!=NULL){
                prev->next = c->next;
            }if(c->next==NULL){
                destroy_client(c);
                clients=NULL;
                return 0;
            }
            else{
                c->next = c->next->next;
                c->name = c->next->name;
                c->id= c->next->id;
                destroy_client(c->next);
                return 0;
            }
            destroy_client(c);
            return 0;
        }
        prev = c;
        c=c->next;
    }
    return -1;
}

Client *retrieve_client(Client *c, int id){
    while (c!=NULL){
        if(c->id == id){
            return c;
        }
        c=c->next;
    }
    return NULL;
}


void print_clients(Client *c){
    printw("Connected Clients\n\n");
    while (c!=NULL){
        printw("name: %s\nid: %d\n\n",c->name,c->id);
        c=c->next;
    }

}

int destroy_client(Client *c){
    free(c->name);
    free(c);
    c=NULL;
    return 0;
}