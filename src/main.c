#include "globals.h"

int init();
int loop();
int quit();


/* This function stops the program by setting the loop var to 0 */
void intHandler(int dummy) {
    run = 0;
}

int main(int argc, char* argv[]){

    signal(SIGINT, intHandler);	/* set sigint to run intHander */
	
    verbose=0;
    state = CLIENT; /* state enum is found in globals.h */

    int i;
    for (i = 0; i < argc; ++i){
        /* check args passed to program */
        if(strcmp(argv[i],"-v")==0){
            verbose=1;
        }
        if(strcmp(argv[i],"-s")==0){
            state=SERVER; /* -s starts a server */
            verbose=1;
        }
    }

    srand(time(NULL)); /* seed the rng */

    run=1;
    init();

    if(run==0){ /* run is 0 if init() exits with an error */
        quit();
        printf("TermChat exited with error.\n"); /* TODO further error handling */
        return -1;
    }

    while(run){
        loop(); /* main loop */
    }

    quit(); /* cleanup */

    return 0;
}

int init(){

    initscr();			/* Start curses mode 		  */
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_RED, COLOR_WHITE);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);
    init_pair(4, COLOR_BLACK, COLOR_CYAN);
    

    getmaxyx( stdscr, screen_height, screen_width );

    switch(state){
        case SERVER:
            cbreak();
            init_server();
        break;
        case CLIENT:;
            cbreak();
            refresh();
            message_box_border = create_newwin(screen_height-6, screen_width, 0,0,3);
            chat_box_border = create_newwin(5, screen_width, screen_height-5,0,3);
            message_box = create_newwin(screen_height-8, screen_width-2, 1,1,1);
            chat_box = create_newwin(3, screen_width-2, screen_height-4,1,1);

            message_rows = screen_height - 8;

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
            raw();
        break;
    }

    return 0;
}

int loop(){
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

    endwin();			/* End curses mode */
    return 0;
}
