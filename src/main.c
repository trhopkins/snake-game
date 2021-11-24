/**
 * Snake game developed for CS 355
 * @author Travis Hopkins
 * CURRENT PROBLEM AT 108
 */

#include <ncurses.h>
#include <stdlib.h>
#define DIR_UP 0
#define DIR_RIGHT 1
#define DIR_DOWN 2
#define DIR_LEFT
int snakestart = 3;

void initializepit();
void makesnake(int);
int detectcollision(int);
void movesnake(int);
void endgame();

struct snake{
    int current_direction; //0 if up, 1 if right, 2 if down, 3 if left
    int x;
    int y;
    struct snake* prev;
};
struct snake* head; struct snake* tail;

int main(void) {
	initscr();              // start ncurses
    curs_set(FALSE);
    cbreak();
    keypad(stdscr, TRUE);
    
    initializepit();        //create border around "snake pit"
    refresh();
    makesnake(KEY_RIGHT);
    refresh();
    int gameend = 1;
	//int input = getchar();                // get user input
    while(gameend){
        //the game is played or something
        int input = getch();                // get user input
        int snakesize = snakestart;
        if(input == KEY_BACKSPACE){
            gameend = 0;
        }
        else
            movesnake(input);
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

//prints the initial snake on the screen and creates the linked list that comprises the snake
void makesnake(int direction){
    head = (struct snake*)malloc(sizeof(struct snake));
    tail = (struct snake*)malloc(sizeof(struct snake));
    head->current_direction = DIR_RIGHT;
    tail->x = COLS/2;
    tail->y = LINES/2;
    struct snake* node= (struct snake*)malloc(sizeof(struct snake));
    tail->prev = node;
    struct snake* prevnode= (struct snake*)malloc(sizeof(struct snake));
    for(int i = 0; i < snakestart ; i++){
        if(direction == KEY_LEFT){
            node->x = tail->x - i; node->y = tail->y;
        }if(direction == KEY_RIGHT){
            node->x = tail->x + i; node->y = tail->y;
        }if(direction == KEY_UP){
            node->x = tail->x; node->y = tail->y - i;
        }if(direction == KEY_DOWN){
            node->x = tail->x; node->y = tail->y + i;
        }
        move(node->y, node->x);
        printw("o");
        mvprintw(i + 1, 1, "%d, %d", node->x, node->y);
        node->prev = prevnode;
        node = prevnode;
    }
    head = node; 
    move(head->y, head->x);
    printw("@"); 
}

void movesnake(int dir){
    mvprintw(tail->y, tail->x, " ");
    mvprintw(head->y, head->x, "o");
    if(dir==KEY_RIGHT){
        if(head->current_direction == DIR_RIGHT){ //WHY DOESN'T THIS WORK
            head->x += 1; tail->x += 1;
        }
        
    }
    else if(dir==KEY_LEFT){
        head->x -= 1; tail->x -= 1;
    }
    else if(dir==KEY_UP){
        head->y -= 1; tail->y -= 1;
    }
    else if(dir==KEY_DOWN){
        head->y += 1; tail->x +=1;
    }
    
    mvprintw(tail->y, tail->x, "o");
    mvprintw(head->y, head->x, "@");    
}

/*returns 0 if there is no character in the next space, and 1 if there is a character
*/
int detectcollision(int direction){
    int testch;
    if(direction == KEY_LEFT){
        testch = mvinch(head->y, head->x + 1);
    }if(direction == KEY_RIGHT){
        testch = mvinch(head->y, head->x - 1);
    }if(direction == KEY_UP){
        testch = mvinch(head->y + 1, head->x);
    }if(direction == KEY_DOWN){
        testch = mvinch(head->y - 1, head->x);
    }
    if(testch == ' '){
        return 0;
    }else{
        return 1;
    }
}
