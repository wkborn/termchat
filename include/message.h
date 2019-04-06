#ifndef _MESSAGE_
#define _MESSAGE_

#define MESSAGE_SIZE    200

typedef struct messageTag{
    struct clientTag *client;
    char *msg;
    struct messageTag *next;
} Message;

Message *create_list(int num, int max_num);
void print_list(Message *e);
void destroy_list(Message *e);
int count_list(Message *e);
int append_message(Message *m, Client *c, char *msg);
int remove_value(Message *e, int val);
#endif
