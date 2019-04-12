#include "globals.h"

WINDOW *create_newwin(int height, int width, int starty, int startx, int cpair)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);

	wbkgd(local_win, COLOR_PAIR(cpair));

	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

void destroy_win(WINDOW *local_win)
{	
	wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' '); 
    wclear(local_win);
	wrefresh(local_win);
	delwin(local_win);
}
