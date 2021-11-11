/**
 * Snake game developed for CS 355
 * @author Travis Hopkins
 */

#include <ncurses.h>
int maxcols, maxrows;
void initializepit();
int main(void) {
	initscr();              // start ncurses

    initializepit();        //create border around "snake pit"
    refresh();

	getch();                // get user input
    
	endwin();               // quit ncurses
	return 0;               // exit program
}

/*creates the border around the playing area and sets the global variables maxcols and maxrows to 
the largest y and x coordinates respectively.
*/
void initializepit(){
    getmaxyx(stdscr, maxcols, maxrows);
    for(int i = 0; i < maxrows; i ++){
        move(0,i);
        printw("-");
        move(maxcols-1, i);
        printw("-");
    }
    for(int i = 0; i < maxcols; i++){
        move(i,0);
        printw("|");
        move(i, maxrows-1);
        printw("|");
    }
}

