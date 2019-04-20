#ifndef _GLOBALS_
#define _GLOBALS_

#include<stdio.h>
#include<stdlib.h>

#include<ncurses.h>

#include <enet/enet.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <strings.h>
#include <string.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>

#include "client.h"
#include "message.h"
#include "networking.h"
#include "networking.h"
#include "server.h"
#include "ncurses_utils.h"

#define MAX_CLIENTS 16


enum STATE {
    SERVER,
    CLIENT
};

enum MESSAGE_TYPE {
    MESSAGE_PEER,
    MESSAGE_NOTICE
};

int screen_width;
int screen_height;

int message_rows;

int run;

int verbose;
int state;

WINDOW *message_box_border;
WINDOW *chat_box_border;
WINDOW *message_box;
WINDOW *chat_box;


#endif