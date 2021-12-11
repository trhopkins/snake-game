/**
 * Snake game developed for CS 355
 * @author Travis Hopkins
 * CURRENT PROBLEM AT 117
 */

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#define DIR_UP KEY_UP
#define DIR_RIGHT KEY_RIGHT
#define DIR_DOWN KEY_DOWN
#define DIR_LEFT KEY_LEFT
int snakesize = 5;

void initializepit();
void makesnake(int);
int detectcollision(int);
void movesnake(int);
void endgame();
void altmakesnake(); 
void altmovesnake(int);
void gameover();
void gamewin();
void growsnake();
int headx, tailx, heady, taily, headdir, taildir;
int win;
struct snake{
    int current_direction; //0 if up, 1 if right, 2 if down, 3 if left
    int x;
    int y;
    struct snake* prev;
};
struct snake* head; struct snake* tail;
struct snake * addsnake(struct snake **, int, int);
int main(void) {
	initscr();              // start ncurses
    curs_set(FALSE);
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    srand(time(0));
    //nodelay(stdscr, TRUE);
    initializepit();        //create border around "snake pit"
    refresh();
    //makesnake(KEY_RIGHT);
    altmakesnake();
     //Seed random function
    refresh();
    int gameend = 1;
    int testcoll;
	//int input = getchar();                // get user input
    mvprintw(2,1, "Before loop: %d, %d   %d, %d", headx, heady, tailx, taily); 
    while(gameend){
        //the game is played or something
        mvprintw(3,1, "before getchar: %d, %d   %d, %d", headx, heady, tailx, taily); 
        int input = getch();                // get user input
        mvprintw(4,1, "after getchar: %d, %d   %d, %d", headx, heady, tailx, taily); 
        
        if(input == KEY_BACKSPACE){
            gameend = 0;
        }
        else{ 
            
            testcoll = detectcollision(input);
            if(testcoll == 1){
                gameover(); 
                refresh();               
                gameend = 0;
            }else if(testcoll == 2){
                altmovesnake(input);
                growsnake();
                if (snakesize >= win){
                    gameend = 0;
                }
                mvprintw(rand() % LINES -1, rand() % COLS -1, "*");
            }else{
                altmovesnake(input);
            }
            
            refresh();
        }
                
        //usleep(500000); //wait half a second
        //altmovesnake(headdir);
        
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
    win = COLS + LINES;
    mvprintw(rand() % LINES -1, rand() % COLS -1, "*"); //Should place a trophy in random spot of pit
}

//prints the initial snake on the screen and creates the linked list that comprises the snake
void makesnake(int direction){
    head = (struct snake*)malloc(sizeof(struct snake));
    tail = (struct snake*)malloc(sizeof(struct snake));
    head->current_direction = DIR_RIGHT;

    tail->x = COLS/2;
    tail->y = LINES/2;
    struct snake* node= (struct snake*)malloc(sizeof(struct snake));
    int x = tail->x; int y = tail->y; 
    head->x = x; head->y = y;
    for(int i = 1; i < snakesize ; i++){
        

        if(direction == KEY_LEFT){
            x = x - 1; 
        }
        else if(direction == KEY_RIGHT){
            x = x - 1; 
        }
        else if(direction == KEY_UP){
            y = y - 1;
        }
        else if(direction == KEY_DOWN){
            y = y + 1;
        }
        move(y, x);
        printw("o");
        node = addsnake(&tail, x, y);

    }
    node->prev = head; //this is the problem. node is tail.prev, so it cuts off most of the linked list
    move(head->y, head->x);
    printw("@");
    
}

void altmakesnake(){
    int randdir = rand() % 4;    
    headx = COLS/2;
    heady = LINES/2;
    taily = LINES/2; tailx = COLS/2;
    if(randdir == 0){
        headdir = KEY_LEFT;
    }else if (randdir == 2){
        headdir = KEY_RIGHT;
    }else if(randdir == 1){
        headdir = KEY_UP;
    }else{
        headdir = KEY_DOWN;
    }
    taildir = headdir;
    
    for(int i = 0; i < snakesize ; i++){

        if(headdir == KEY_LEFT){
            tailx +=1;
            mvprintw(heady, headx + i , "<");
            
        }
        else if(headdir == KEY_RIGHT){
            tailx-=1;
            mvprintw(heady, headx - i, ">");
            
        }
        else if(headdir == KEY_UP){
            taily += 1;
            mvprintw(heady + i, headx, "^");
            
        }
        else if(headdir == KEY_DOWN){
            taily -= 1;
            mvprintw(heady - i, headx, "v");
            
        }
    }
    move(heady, headx);
    printw("@");
    mvprintw(1,1, "In makesnake: %d, %d   %d, %d", headx, heady, tailx, taily);    
    
}

void altmovesnake(int direction){
    mvprintw(4,1, "in movesnake: %d, %d   %d, %d taildir = %d", headx, heady, tailx, taily, taildir);
    
    if(direction == KEY_LEFT){
        mvprintw(heady, headx, "<");
    }
    else if(direction == KEY_RIGHT){
        mvprintw(heady, headx, ">");
    }
    else if(direction == KEY_UP){
        mvprintw(heady, headx, "^");
    }
    else if(direction == KEY_DOWN){
        mvprintw(heady, headx, "v");
    }
    headdir= direction;


    if(direction == KEY_LEFT){
        headx -= 1;
        mvprintw(heady, headx, "@");
    }
    else if(direction == KEY_RIGHT){
        headx += 1;
        mvprintw(heady, headx, "@");
    }
    else if(direction == KEY_UP){
        heady -= 1;
        mvprintw(heady, headx, "@");
    }
    else if(direction == KEY_DOWN){
        heady += 1;
        mvprintw(heady, headx, "@");
    }   
    mvprintw(taily, tailx, " ");
    if(taildir == KEY_LEFT){
        tailx -= 1;
    }
    else if(taildir == KEY_RIGHT){
        tailx += 1;
    }
    else if(taildir == KEY_UP){
        taily -= 1;
    }
    else if(taildir == KEY_DOWN){
        taily += 1;
    }
 
    int test;
    if(taildir == KEY_LEFT){
        test = mvinch(taily, tailx);
    }
    else if(taildir == KEY_RIGHT){
        test = mvinch(taily, tailx);
    }
    else if(taildir == KEY_UP){
        test = mvinch(taily, tailx);
    }
    else if(taildir == KEY_DOWN){
        test = mvinch(taily, tailx);
    }
    
    if(test == '<'){
        taildir = KEY_LEFT;        
    }else if(test == '>'){
        taildir = KEY_RIGHT;
    }else if(test == '^'){
        taildir = KEY_UP;
    }else if(test == 'v'){
        taildir = KEY_DOWN;
    }    
    
}

void movesnake(int dir){
    mvprintw(tail->y, tail->x, " "); //Tail is not properly working
    mvprintw(head->y, head->x, "o");
    if(dir==KEY_RIGHT){
        /*if(head->current_direction == DIR_RIGHT){ //WHY DOESN'T THIS WORK
            head->x += 1; tail->x += 1;
        }*/
        
        struct snake* newhead = (struct snake*)malloc(sizeof(struct snake));
        newhead->x = head->x +1; newhead->y = head->y;
        head->prev = newhead;
        head = newhead;
        tail = tail->prev;
        head->current_direction = DIR_RIGHT;
        
    }
    else if(dir==KEY_LEFT){
        struct snake* newhead = (struct snake*)malloc(sizeof(struct snake));
        newhead->x = head->x -1; newhead->y = head->y;
        head->prev = newhead;
        head = newhead;
        tail = tail->prev;
        head->current_direction = DIR_LEFT;
    }
    else if(dir==KEY_UP){
        struct snake* newhead = (struct snake*)malloc(sizeof(struct snake));
        newhead->x = head->x; newhead->y = head->y -1;
        head->prev = newhead;
        head = newhead;
        tail = tail->prev;
        head->current_direction = DIR_UP;
    }
    else if(dir==KEY_DOWN){
        struct snake* newhead = (struct snake*)malloc(sizeof(struct snake));
        newhead->x = head->x; newhead->y = head->y +1;
        head->prev = newhead;
        head = newhead;
        tail = tail->prev;
        head->current_direction = DIR_DOWN;
    }
    
    mvprintw(tail->y, tail->x, "o");
    mvprintw(head->y, head->x, "@");    
}

/*returns 0 if there is no character in the next space, and 1 if there is a character
*/
int detectcollision(int direction){
    int testch;
    if(direction == KEY_LEFT){
        testch = mvinch(heady, headx - 1);
    }if(direction == KEY_RIGHT){
        testch = mvinch(heady, headx + 1);
    }if(direction == KEY_UP){
        testch = mvinch(heady - 1, headx);
    }if(direction == KEY_DOWN){
        testch = mvinch(heady + 1, headx);
    }
    if(testch == ' '){
        return 0;
    }else if (testch == '*'){
        return 2;
    }
    else{
        return 1;
    }
}
void gameover(){
    mvprintw(LINES/2, (COLS/2)-10, "Game Over! Score: %d", snakesize);
    mvprintw(LINES/2 + 1, (COLS/2) - 11, "Press any key to exit");
    refresh();
    getchar();
}

void gamewin(){
    mvprintw(LINES/2 - 15, COLS/2, "You won! Press any key to exit");
    refresh();
    getchar();
}

struct snake * addsnake(struct snake ** node, int x, int y){
    struct snake * newnode = (struct snake*)malloc(sizeof(struct snake));
    newnode->x = x;
    newnode->y = y;
    newnode->prev = *node;
    *node = newnode;
    return newnode;
}
void growsnake(){
    if(taildir == KEY_LEFT){
        tailx += 1;
        mvprintw(taily, tailx, "<");
    }else if(taildir == KEY_RIGHT){
        tailx -= 1;
        mvprintw(taily, tailx, ">");
    }else if(taildir == KEY_UP){
        taily += 1;
        mvprintw(taily, tailx, "^");
    }else if (taildir == KEY_DOWN){
        taily -= 1;
        mvprintw(taily, tailx, "v");
    }
snakesize += 1;    
}