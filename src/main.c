#include "globals.h"


enum STATE {
    SERVER,
    CLIENT
};

int verbose=0;
int state = CLIENT;

int init();
int actions();
int quit();
void intHandler(int dummy) {
    run = 0;
}

int main(int argc, char* argv[]){

    signal(SIGINT, intHandler);

    int i;
    for (i = 0; i < argc; ++i){
        
        if(strcmp(argv[i],"-v")==0){
            verbose=1;
        }
        if(strcmp(argv[i],"-s")==0){
            state=SERVER;
            verbose=1;
        }
    }

    srand(time(NULL));

    init();

    run=1;
    while(run){
        actions();
    }

    quit();

    return 0;
}

int init(){

    switch(state){
        case SERVER:
            init_server();
        break;
        case CLIENT:;
            char address[NETBUFFER_SIZE];
            printf("Server Address: ");
            fgets( address, NETBUFFER_SIZE, stdin );
            strtok(address, "\n");
            if(init_client(address)<0){
                run=0;
                return -1;
            }
        break;
    }

    return 0;
}

int actions(){
    switch(state){
        case SERVER:
            server_actions();
        break;
        case CLIENT:;
            client_actions();
        break;
    }
    return 0;
}

int quit(){

    printf("\nexiting\n");

    switch(state){
        case SERVER:
            server_actions();
        break;
        case CLIENT:;
            disconnect();
            deinit_client();
        break;
    }
    return 0;
}