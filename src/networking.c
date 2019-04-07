#include "globals.h"

int unpack_handshake(ENetEvent e){
  char *saveptr;
  strtok_r((char*)e.packet->data ,":", &saveptr);
  return 0;
}



int message_parse(ENetEvent e){
    char *saveptr;
    int id;
    char* msg;
    // Now parse and assign data fields
    strtok_r((char*)e.packet->data ,":", &saveptr); // JUST THE OPCODE, can be thrown away
    id = atoi(strtok_r(NULL, ":", &saveptr));
    msg = strtok_r(NULL, ":", &saveptr);
    Client *this = retrieve_client(clients,id);
    if(this==NULL){
        printw("illegal client\n");
        return -1;
    }
    printw("<%s> %s\n",this->name,msg);

    append_message(messages,this,msg);

  return 0;
}

int unpack_messages(ENetEvent e){
  char *saveptr;
  strtok_r((char*)e.packet->data ,":", &saveptr);  // JUST THE OPCODE, can be thrown away

  return 0;
}
