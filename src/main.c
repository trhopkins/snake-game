/**
 * Snake game developed for CS 355
 * @author Travis Hopkins
 */

#include <ncurses.h>
int snakestart = 3; int snakex; int snakey;

void initializepit();
void makesnake();

int main(void) {
	initscr();              // start ncurses
    curs_set(FALSE);
    cbreak();
    keypad(stdscr, TRUE);
    
    initializepit();        //create border around "snake pit"
    refresh();
    makesnake(KEY_RIGHT);
    refresh();

    int gameend = 0;
	int input = getchar();                // get user input
    while(gameend){
        //the game is played or something
    }

	endwin();               // quit ncurses
	return 0;               // exit program
}

/*creates the border around the playing area 
*/
void initializepit(){
    for(int i = 0; i < COLS; i ++){
        move(0,i);
        printw("-");
        move(LINES-1, i);
        printw("-");
    }
    for(int i = 0; i < LINES; i++){
        move(i,0);
        printw("|");
        move(i, COLS-1);
        printw("|");
    }
}

//prints the initial snake on the screen
void makesnake(int direction){
    snakex = COLS/2;
    snakey = LINES/2;    
    move(snakey, snakex);
    printw("@");
    for(int i = 1; i < snakestart; i++){
        if(direction == KEY_LEFT){
            move(snakey, snakex + i);
        }if(direction == KEY_RIGHT){
            move(snakey, snakex - i);
        }if(direction == KEY_UP){
            move(snakey + i, snakex);
        }if(direction == KEY_DOWN){
            move(snakey - i, snakex + i);
        }
        printw("o");
    }
}

