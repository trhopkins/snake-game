/**
 * Snake game developed for CS 355
 * @author Travis Hopkins
 */

#include <ncurses.h>

int main(void) {
	initscr();              // start ncurses
	addstr("Hello world!"); // add message to buffer
	refresh();              // show buffer
	getch();                // get user input
	endwin();               // quit ncurses
	return 0;               // exit program
}

