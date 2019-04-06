#include "globals.h"

int init_client(char* host_ip){
    
    srand(time(NULL));

    this_client = calloc(1,sizeof(Client));
    this_client->id=rand();
    this_client->name = calloc(BUFFER_SIZE, sizeof(char));
    printf("Client name: ");
    fgets( this_client->name, BUFFER_SIZE, stdin );
    strtok(this_client->name, "\n");
    this_client->next = NULL;

    printf("Client created:\nname: %s\nid: %d\n",this_client->name, this_client->id);

    recieved_handshake=0;

    if(enet_initialize() != 0){
        printf("Could not initialize networking.\n");
        return -1;
    }

    client = enet_host_create(NULL, 1, 0, 5760/8, 1440/8);

    if(client == NULL){
        printf("Could not create client.\n");
        return -1;
    }

    address = NULL;
    address = calloc(1,sizeof(ENetAddress));

    address->host = ENET_HOST_ANY;
    address->port = PORT;

    enet_address_set_host(address, host_ip);
    address->port = PORT;

    peer = enet_host_connect(client, address, 0, 0);
    if(peer == NULL){
        printf("could not open connection on client");
        return -1;
    }else
    {
        printf("connecting to %s ...\n", host_ip);
    }

    if(enet_host_service(client, &event, 1000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT){
        initial_connect();
        printf("handshaked. \n");
    }else{
        printf("could not connect to %s\n", host_ip);
        return -1;
    }

    if(!recieved_handshake){
        initial_connect();
    }

    run=1;
    pthread_create( &chat_thread, NULL, chat_handler, NULL);

    printf("client initialized\n");
    return 0;
}

void * chat_handler( void *ptr ){
    while (run){
        char message[MESSAGE_SIZE];
        printf("Chat> ");
        fgets( message, MESSAGE_SIZE, stdin );
        strtok(message, "\n");
        snprintf(net_buffer,NETBUFFER_SIZE,"%d:%d:%s:", MESSAGE, this_client->id, message);
        send_server_message();
    }
    return NULL;
}

int initial_connect(){
    snprintf(net_buffer,NETBUFFER_SIZE,"%d:%d:%s",CONNECT,this_client->id,this_client->name);
    printf("sending: %s\n", net_buffer);
    send_server_message();
    return 0;
}

int update_server(){
    // Send player info
    send_server_message();
    return 0;
}

int client_actions(){
    client_event_handle();
    return 0;
}


int client_event_handle(){
    while(enet_host_service(client, &event, 0) > 0){
        switch(event.type){
            case ENET_EVENT_TYPE_CONNECT: break;
            case ENET_EVENT_TYPE_RECEIVE:
                client_parse_packet(event);
                break;
            case ENET_EVENT_TYPE_DISCONNECT: break;
            default:
                break;
        }
    }
    return 0;
}

int client_parse_packet(ENetEvent e){
    char* packet_data = (char*)e.packet->data;
    sprintf(net_buffer, "%c", packet_data[0]);
    int opcode = atoi(net_buffer);
    switch(opcode){
        case ACTION:
            // Update the currently connected player arrays
        break;
        case DISCONNECT:
        break;
        case HANDSHAKE:
          if (!recieved_handshake) {
            unpack_handshake(e);
            recieved_handshake=1;
          }
        break;
        case MESSAGE:;
            char *saveptr;
            char* name;
            char* msg;
            // Now parse and assign data fields
            strtok_r((char*)e.packet->data ,":", &saveptr); // JUST THE OPCODE, can be thrown away
            name = strtok_r(NULL, ":", &saveptr);
            msg = strtok_r(NULL, ":", &saveptr);
            printf("\n<%s> %s\n",name,msg);
        break;
    }
    return 0;
}

int send_server_message(){
    packet = enet_packet_create(net_buffer, strlen(net_buffer)+1, ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(peer, 0, packet);
    return 0;
}

int disconnect(){
    snprintf(net_buffer,NETBUFFER_SIZE,"%d:%d:",DISCONNECT,this_client->id);
    printf("sending: %s\n", net_buffer);
    send_server_message();
    client_event_handle();
  return 0;
}

int deinit_client(){
    pthread_cancel(chat_thread);
    pthread_join( chat_thread, NULL);
    enet_host_destroy(client);
    client=NULL;
    enet_deinitialize();
    client_online = 0;
    return 0;
}
