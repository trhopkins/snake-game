/**
 * Snake game developed for CS 355
 * @author Harry Schmitt
 */

#include <ncurses.h>
#include <stdio.h>
int snakestart = 5, snakey, snakex;

    

void initializepit();
void makesnake(int*);
void movesnake(int, int*, int);

int main(void) {
    int gameend = 1; //game runs when 1, exits when 0
    int snake_coord[2] = {snakex, snakey};

	initscr();              // start ncurses
    curs_set(FALSE);
    noecho();    //turns off echo
    raw();       //turns off canonical mode
    cbreak();
    keypad(stdscr, TRUE);
    
    initializepit();        //create border around "snake pit"
    refresh();
    makesnake(snake_coord);
    refresh();

    

    while(gameend){
        //the game is played or something        
        int input = getch();                // get user input
        int snakesize = snakestart;
        if(input == KEY_BACKSPACE){
            gameend = 0;
        }
        else
            movesnake(input, snake_coord, snakesize);
        refresh();        
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
void makesnake(int snake_coord[]){
    snake_coord[0] = (COLS/2)-snakestart; //x axis
    snake_coord[1] = LINES/2; //y axis
    move(snake_coord[1], snake_coord[0]);
    for(int i = 0; i < snakestart; i++){
        snake_coord[0] += 1;        
        mvprintw(snake_coord[1], snake_coord[0], "o");
    }
    printw("@");
    snake_coord[0] += 1;    
    
}

void movesnake(int direction, int* coordinates, int size){
    mvprintw(coordinates[1], coordinates[0],"o");    
    if(direction == KEY_LEFT){
        coordinates[0] -= 1;
    }
    else if(direction == KEY_RIGHT){
        
        mvprintw(coordinates[1], coordinates[0] - size, " ");
        coordinates[0] += 1;
    }
    else if(direction == KEY_UP){
        coordinates[1] -=1;
    }
    else if(direction == KEY_DOWN){
        coordinates[1] += 1;
    }
    mvprintw(coordinates[1], coordinates[0], "@");
}

