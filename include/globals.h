#include<stdio.h>
#include<stdlib.h>
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
#include "message.h"
#include "networking.h"
#include "server.h"



#define MAX_CLIENTS 16