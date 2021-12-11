/**
 * Snake game developed for CS 355
 */

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define DIR_UP KEY_UP
#define DIR_RIGHT KEY_RIGHT
#define DIR_DOWN KEY_DOWN
#define DIR_LEFT KEY_LEFT

int snakestart = 5;

struct snake {
    int current_direction; //See Definitions above
    int x;
    int y;
    struct snake* prev;
};

void initializepit();
void makesnake(int);
int detectcollision(int, struct snake[]);
void movesnake(int);
void endgame();
void altmakesnake(int, struct snake[]);
void altmovesnake(int, struct snake[]);
void gameover();
void gamewin();
void growsnake(struct snake[]);

int win;
int headdir;
int headx, tailx, heady, taily, taildir;


struct snake* head; struct snake* tail;
struct snake * addsnake(struct snake **, int, int);

int main(void) {
    initscr();              // start ncurses
    curs_set(FALSE);
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    nodelay(stdscr, TRUE);
    srand(time(0));
    initializepit();        //create border around "snake pit"
    refresh();

    struct snake snake_array[((COLS * 2) + (LINES * 2))/2];
    altmakesnake(KEY_RIGHT, snake_array);
    for(int i = 0; i < snakestart; i ++){
        mvprintw(6 + i, 1, "[%d]", snake_array[i].x);
    }
    refresh();
    int gameend = 1;
    mvprintw(2,1, "Before loop: %d, %d   %d, %d", headx, heady, tailx, taily);

    while (gameend) {
        //the game is played or something
        mvprintw(3,1, "before getchar: %d, %d   %d, %d", headx, heady, tailx, taily);
        int input = getch();                // get user input
        mvprintw(4,1, "after getchar: %d, %d   %d, %d", headx, heady, tailx, taily);
        int snakesize = snakestart;

        if (input == KEY_BACKSPACE) {
            gameend = 0;
        } else if (input == KEY_LEFT || input == KEY_RIGHT || input == KEY_DOWN || input == KEY_UP) {
            int testcoll = detectcollision(input, snake_array);
            if(testcoll == 1){
                gameover(); 
                refresh();               
                gameend = 0;
            }else if(testcoll == 2){
                altmovesnake(input, snake_array);
                growsnake(snake_array);
                if (snakestart >= win){
                    gameend = 0;
                }
                mvprintw(rand() % LINES -1, rand() % COLS -1, "*");
            }else{
                altmovesnake(input, snake_array);
            }         
            snake_array[0].current_direction = input; taildir = input;
        } else {
            int testcoll = detectcollision(snake_array[0].current_direction, snake_array);
            if(testcoll == 1){
                gameover(); 
                refresh();               
                gameend = 0;
            }else if(testcoll == 2){
                altmovesnake(snake_array[0].current_direction, snake_array);
                growsnake(snake_array);
                if (snakestart >= win){
                    gameend = 0;
                }
                mvprintw(rand() % LINES -1, rand() % COLS -1, "*");
            }else{
                altmovesnake(snake_array[0].current_direction, snake_array);
            }
        }

        usleep(500000); //wait half a second
        refresh();
    }
    endwin();               // quit ncurses
    return 0;               // exit program
}

// creates the border around the playing area
void initializepit() {
    win = LINES + COLS;
    for (int i = 0; i < COLS; i ++) {
        move(0,i);
        printw("-");
        move(LINES-1, i);
        printw("-");
    }
    for (int i = 0; i < LINES; i++) {
        move(i,0);
        printw("|");
        move(i, COLS-1);
        printw("|");
    }
    mvprintw(rand() % LINES -1, rand() % COLS -1, "*"); //Should place a trophy in random spot of pit
}

//prints the initial snake on the screen and creates the linked list that comprises the snake
void makesnake(int direction) {
    head = (struct snake*)malloc(sizeof(struct snake));
    tail = (struct snake*)malloc(sizeof(struct snake));
    head->current_direction = DIR_RIGHT;

    tail->x = COLS/2;
    tail->y = LINES/2;
    struct snake* node= (struct snake*)malloc(sizeof(struct snake));
    int x = tail->x; int y = tail->y;
    head->x = x; head->y = y;
    for (int i = 1; i < snakestart ; i++) {

        if (direction == KEY_LEFT) {
            x = x - 1;
        } else if (direction == KEY_RIGHT) {
            x = x - 1;
        } else if (direction == KEY_UP) {
            y = y - 1;
        } else if (direction == KEY_DOWN) {
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

void altmakesnake(int direction, struct snake array[]) {
    array[0].x = COLS/2; //head set to vertical center
    array[0].y = LINES/2; //head set to horizonal center
    //array[1].y = LINES/2; array[1].x = COLS/2; //tail, the next element after head, set to horizontal and vertical center
    array[0].current_direction = direction; //array[1].current_direction = direction; //head and tail current direction set to passed direction

    for (int i = 1; i < snakestart ; i++) {
        array[i].y = LINES/2; array[i].x  = COLS/2;
        if (direction == KEY_LEFT) {
            array[i].x += i;
            mvprintw(array[i].y, array[i].x, "<");

        } else if (direction == KEY_RIGHT) {
            array[i].x -= i;

            mvprintw(array[i].y, array[i].x, ">");

        } else if (direction == KEY_UP) {
            array[i].y += i;
            mvprintw(array[i].y, array[i].x, "^");

        } else if (direction == KEY_DOWN) {
            array[i].y -= i;
            mvprintw(array[i].y, array[i].x, "v");

        }
        //array[i+1].x = array[i].x; array[i+1].y = array[i].y; 
        array[i].current_direction = direction;
    }
    move(array[0].y, array[0].x);
    printw("@");
    mvprintw(1,1, "In makesnake: %d, %d   %d, %d", array[0].x, array[0].y, array[snakestart-1].x, array[snakestart-1].y);

}

void altmovesnake(int direction, struct snake array[]) {
    mvprintw(4,1, "in movesnake: %d, %d   %d, %d taildir = %d", array[0].x, array[0].y, array[snakestart-1].x, array[snakestart-1].y, array[snakestart-1].current_direction);
    
    mvprintw(array[snakestart-1].y, array[snakestart-1].x, " ");
    for(int i = snakestart - 1; i > 0; i--){
        array[i].x = array[i-1].x; array[i].y = array[i-1].y; //Each element's position is set to the position of the element before it
    }
    //prints an arrow in place of head, sets head direction
    if (direction == KEY_LEFT) {
        mvprintw(array[0].y, array[0].x, "<");
    } else if (direction == KEY_RIGHT) {
        mvprintw(array[0].y, array[0].x, ">");
    } else if (direction == KEY_UP) {
        mvprintw(array[0].y, array[0].x, "^");
    } else if (direction == KEY_DOWN) {
        mvprintw(array[0].y, array[0].x, "v");
    }
    array[0].current_direction = direction;
    //moves position of head, prints head
    if (direction == KEY_LEFT) {
        array[0].x -= 1;
        mvprintw(array[0].y, array[0].x, "@");
    } else if (direction == KEY_RIGHT) {
        array[0].x += 1;
        mvprintw(array[0].y, array[0].x, "@");
    } else if (direction == KEY_UP) {
        array[0].y -= 1;
        mvprintw(array[0].y, array[0].x, "@");
    } else if (direction == KEY_DOWN) {
        array[0].y += 1;
        mvprintw(array[0].y, array[0].x, "@");
    }
    
    //Tail is moved
   /* if (array[snakestart].current_direction == KEY_LEFT) {
        array[snakestart].x -= 1;
    } else if (array[snakestart-1].current_direction == KEY_RIGHT) {
        array[snakestart].x += 1;
    } else if (array[snakestart-1].current_direction == KEY_UP) {
        array[snakestart].y -= 1;
    } else if (array[snakestart-1].current_direction == KEY_DOWN) {
        array[snakestart].y += 1;
    }*/
    
/*
    int test;
    if (array[snakestart].current_direction == KEY_LEFT) {
        test = mvinch(array[snakestart].y, array[snakestart].x);
    } else if (array[snakestart].current_direction == KEY_RIGHT) {
        test = mvinch(array[snakestart].y, array[snakestart].x);
    } else if (array[snakestart].current_direction == KEY_UP) {
        test = mvinch(array[snakestart].y, array[snakestart].x);
    } else if (array[snakestart].current_direction == KEY_DOWN) {
        test = mvinch(array[snakestart].y, array[snakestart].x);
    }

    if (test == '<') {
        array[snakestart].current_direction = KEY_LEFT;
    } else if (test == '>') {
        array[snakestart].current_direction = KEY_RIGHT;
    } else if (test == '^') {
        array[snakestart].current_direction = KEY_UP;
    } else if (test == 'v') {
        array[snakestart].current_direction = KEY_DOWN;
    }
    */

}

void movesnake(int dir) {
    mvprintw(tail->y, tail->x, " "); //Tail is not properly working
    mvprintw(head->y, head->x, "o");
    if (dir==KEY_RIGHT) {
        /*if(head->current_direction == DIR_RIGHT){ //WHY DOESN'T THIS WORK
            head->x += 1; tail->x += 1;
        }*/

        struct snake* newhead = (struct snake*)malloc(sizeof(struct snake));
        newhead->x = head->x +1; newhead->y = head->y;
        head->prev = newhead;
        head = newhead;
        tail = tail->prev;
        head->current_direction = DIR_RIGHT;

    } else if (dir==KEY_LEFT) {
        struct snake* newhead = (struct snake*)malloc(sizeof(struct snake));
        newhead->x = head->x -1; newhead->y = head->y;
        head->prev = newhead;
        head = newhead;
        tail = tail->prev;
        head->current_direction = DIR_LEFT;
    } else if (dir==KEY_UP) {
        struct snake* newhead = (struct snake*)malloc(sizeof(struct snake));
        newhead->x = head->x; newhead->y = head->y -1;
        head->prev = newhead;
        head = newhead;
        tail = tail->prev;
        head->current_direction = DIR_UP;
    } else if (dir==KEY_DOWN) {
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
int detectcollision(int direction, struct snake array[]){
    int testch;
    if(direction == KEY_LEFT){
        testch = mvinch(array[0].y, array[0].x- 1);
    }if(direction == KEY_RIGHT){
        testch = mvinch(array[0].y, array[0].x + 1);
    }if(direction == KEY_UP){
        testch = mvinch(array[0].y - 1, array[0].x);
    }if(direction == KEY_DOWN){
        testch = mvinch(array[0].y + 1, array[0].x);
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

struct snake * addsnake(struct snake ** node, int x, int y){
    struct snake * newnode = (struct snake*)malloc(sizeof(struct snake));
    newnode->x = x;
    newnode->y = y;
    newnode->prev = *node;
    *node = newnode;
    return newnode;
}
void gameover(){
    mvprintw(LINES/2, (COLS/2)-10, "Game Over! Score: %d", snakestart);
    mvprintw(LINES/2 + 1, (COLS/2) - 11, "Press any key to exit");
    refresh();
    getchar();
}

void gamewin(){
    mvprintw(LINES/2 - 15, COLS/2, "You won! Press any key to exit");
    refresh();
    getchar();
}

void growsnake(struct snake array[]){
    
    snakestart += 1; array[snakestart - 1].x = array[snakestart - 2].x;array[snakestart - 1].y = array[snakestart - 2].y;array[snakestart - 1].current_direction = array[snakestart - 2].current_direction;
    
    if(array[snakestart - 1].current_direction == KEY_LEFT){
        
        array[snakestart - 1].x += 1;
        mvprintw(array[snakestart - 1].y, array[snakestart - 1].y, "<");
    }else if(array[snakestart - 1].current_direction == KEY_RIGHT){
        array[snakestart - 1].x -= 1;
        mvprintw(array[snakestart - 1].y, array[snakestart - 1].x, ">");
    }else if(array[snakestart - 1].current_direction == KEY_UP){
        array[snakestart - 1].y += 1;
        mvprintw(array[snakestart - 1].y, array[snakestart - 1].x, "^");
    }else if (array[snakestart - 1].current_direction== KEY_DOWN){
        array[snakestart - 1].y -= 1;
        mvprintw(array[snakestart - 1].y, array[snakestart - 1].x, "v");
    } 
}
