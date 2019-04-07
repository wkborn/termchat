#include "globals.h"

Message *create_list(int num, int max_num){
    if(num<2){
        printw("%s","Must create list of more than 2 messages.\n");
        return NULL;
    }
    Message *first = calloc(1,sizeof(Message));
    first->msg= calloc(MESSAGE_SIZE,sizeof(char));
    snprintf(first->msg, MESSAGE_SIZE, "%d", rand() % max_num);
    Message *subsequent = calloc (1,sizeof(Message));
    subsequent->msg= calloc(MESSAGE_SIZE,sizeof(char));
    snprintf(first->msg, MESSAGE_SIZE, "%d", rand() % max_num);
    first->next = subsequent;
    for (int i=2; i<num;i++){
        Message *previous = subsequent;
        subsequent = calloc(1,sizeof(Message));
        subsequent->msg= calloc(MESSAGE_SIZE,sizeof(char));
        snprintf(first->msg, MESSAGE_SIZE, "%d", rand() % max_num);
        previous->next=subsequent;
    }
    subsequent->next=NULL;
    return first;
}

void print_list(Message *e) {
    if(e==NULL){
        printw("Cannot print. List does not exist.");
    }
    printw("List:\n-----\n");
    while(e != NULL){
        printw("%s \n",e->msg);
        e=e->next;
    }
}

void destroy_list(Message *e){
    while(e != NULL){
        Message *next = e->next;
        free(e);
        e=next;
    }
}

int append_message(Message *m, Client *c, char *msg){
    if(m == NULL){
        messages = calloc(1,sizeof(Message));
        messages->client = c;
        messages->msg= calloc(MESSAGE_SIZE,sizeof(char));
        snprintf(messages->msg,MESSAGE_SIZE,"%s",msg);
        messages->next = NULL;
        return 0;
    }
    Message *old_msg;
    old_msg= messages;
    messages = calloc(1,sizeof(Message));
    messages->client = c;
    messages->msg= calloc(MESSAGE_SIZE,sizeof(char));
    snprintf(messages->msg,MESSAGE_SIZE,"%s",msg);
    messages->next = old_msg;
    
    return -1;
}

int count_list(Message *e){
    int count;
    while(e!=NULL){
        count++;
        e=e->next;
    }
    return count;
}


int remove_value(Message *e, int val){
    
    //TODO remove message

    return 0;
}