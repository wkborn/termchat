#include "globals.h"

int init_client(char* host_ip){
    
    srand(time(NULL));

    this_client = calloc(1,sizeof(Client));
    this_client->id=rand();
    this_client->name = calloc(BUFFER_SIZE, sizeof(char));
    wprintw(message_box,"Client name: ");
    wrefresh(message_box);
    wscanw(message_box, "%s", this_client->name );
    strtok(this_client->name, "\n");
    this_client->next = NULL;

    wprintw(message_box,"Client created:\nname: %s\nid: %d\n",this_client->name, this_client->id);
    wrefresh(message_box);

    recieved_handshake=0;

    if(enet_initialize() != 0){
        wprintw(message_box,"Could not initialize networking.\n");
        wrefresh(message_box);
        return -1;
    }

    client = enet_host_create(NULL, 1, 0, 5760/8, 1440/8);

    if(client == NULL){
        wprintw(message_box,"Could not create client.\n");
        wrefresh(message_box);
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
        wprintw(message_box,"could not open connection on client");
        wrefresh(message_box);
        return -1;
    }else
    {
        wprintw(message_box,"connecting to %s ...\n", host_ip);
        wrefresh(message_box);
    }

    if(enet_host_service(client, &event, 1000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT){
        initial_connect();
        wprintw(message_box,"handshaked. \n");
        wrefresh(message_box);
    }else{
        wprintw(message_box,"could not connect to %s\n", host_ip);
        wrefresh(message_box);
        return -1;
    }

    if(!recieved_handshake){
        initial_connect();
    }

    run=1;
    to_send = calloc(MESSAGE_SIZE,sizeof(char));

    wprintw(message_box,"client initialized\n");
    wrefresh(message_box);
    wclear(message_box);
    return 0;
}

int chat_handler( ){
    wclear(chat_box);
    wrefresh(chat_box);
    snprintf(net_buffer,NETBUFFER_SIZE,"%d:%d:%s:", MESSAGE, this_client->id, to_send);
    send_server_message();
    free(to_send);
    to_send = NULL;
    to_send = calloc(MESSAGE_SIZE, sizeof(char));
    return 0;
}

int initial_connect(){
    snprintf(net_buffer,NETBUFFER_SIZE,"%d:%d:%s",CONNECT,this_client->id,this_client->name);
    wprintw(message_box,"sending: %s\n", net_buffer);
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
    //chat_handler();

    input_handle();

    wclear(chat_box);
    wprintw(chat_box,"%s",to_send);

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
            wprintw(message_box,"<%s> %s\n",name,msg);
            wrefresh(message_box);
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
    wprintw(message_box,"sending: %s\n", net_buffer);
    send_server_message();
    client_event_handle();
  return 0;
}

int deinit_client(){
    enet_host_destroy(client);
    client=NULL;
    enet_deinitialize();
    client_online = 0;
    return 0;
}

int input_handle(){
    int c = wgetch(chat_box);
    //printf(" <%d> ",c);
    //wprintw(message_box," <%d> ",c);
    //wrefresh(message_box);
    switch(c)
    {	
        case -1:
            ;//do nothing
        break;
        case 10:
            chat_handler();
            break;
        case 127:;
            int i;
            for (i=0;i<MESSAGE_SIZE;i++)
              {
                if(to_send[i]=='\0' && i>0)
                {
                  to_send[i-1]='\0';
                }
              }
            break;
        case 27:
            run = 0;
            break;
        default:
            if(1){
                char *to_cat = calloc(1,sizeof(char));
                snprintf(to_cat,2,"%c",c);
                strcat(to_send,to_cat);
                free(to_cat);
                to_cat=NULL;
            }
            break;
    }

    return 0;
}
