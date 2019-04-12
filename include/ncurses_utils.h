#ifndef _NCURSESUTILS_
#define _NCURSESUTILS_

#include<ncurses.h>

WINDOW *create_newwin(int height, int width, int starty, int startx, int cpair);
void destroy_win(WINDOW *local_win);

#endif
