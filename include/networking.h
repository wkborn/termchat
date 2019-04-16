#ifndef _NETWORKING_
#define _NETWORKING_

#define NETBUFFER_SIZE  1000
#define BUFFER_SIZE 200

#define PORT 1337


Message *messages;

int glob_online;
int client_online;
ENetPeer *peer;
ENetEvent event;
ENetPacket *packet;
ENetAddress *address;

char net_buffer[NETBUFFER_SIZE];

enum OPCODES {
  CONNECT,
  ACTION,
  MESSAGE,
  DISCONNECT,
  HANDSHAKE,
  UPDATE_CONNECTED_PLAYERS,
  UPDATE_MESSAGES,
  SERVER_NOTICE
};

enum COLORS {
  WHITE,
  BLACK,
  RED,
  ORANGE,
  YELLOW,
  GREEN,
  CYAN,
  BLUE,
  PURPLE,
  PINK
};

int unpack_handshake(ENetEvent e);
int message_parse(ENetEvent e);
int unpack_messages(ENetEvent e);
#endif
