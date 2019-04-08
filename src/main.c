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

    initscr();			/* Start curses mode 		  */
    cbreak();

    getmaxyx( stdscr, screen_height, screen_width );

    switch(state){
        case SERVER:
            init_server();
        break;
        case CLIENT:;
            refresh();
            message_box_border = create_newwin(screen_height-6, screen_width, 0,0);
            chat_box_border = create_newwin(5, screen_width, screen_height-5,0);
            message_box = create_newwin(screen_height-8, screen_width-2, 1,1);
            chat_box = create_newwin(3, screen_width-2, screen_height-4,1);
            wclear(message_box);
            wclear(chat_box);
            char address[NETBUFFER_SIZE];
            wprintw(message_box,"Server Address: ");
            wrefresh(message_box);
            wrefresh(chat_box);	
            
            wtimeout(chat_box,100);

            echo();			/* Wait for user input */
            wscanw( message_box, "%s", address );
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

    refresh();			/* Print it on to the real screen */
    return 0;
}

int quit(){

    printf("\nexiting\n");
 
    switch(state){
        case SERVER:
            server_actions();
        break;
        case CLIENT:;
            destroy_win(message_box_border);
            destroy_win(chat_box_border);
            destroy_win(message_box);
            destroy_win(chat_box);
            disconnect();
            deinit_client();
        break;
    }

    endwin();			/* End curses mode		  */
    return 0;
}