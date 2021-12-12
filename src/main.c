/**
 * Snake game developed for CS 355
 */

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


int snakestart = 5;
int trophyValue;
int trophyExpiration;
int trophyX;
int trophyY;

struct snake {
    int current_direction; 
    int x;
    int y;
};

void setTrophy();
void initializepit();
int detectcollision(int, struct snake[]);
void altmakesnake(int, struct snake[]);
void altmovesnake(int, struct snake[]);
void gameover();
void gamewin();
void growsnake(struct snake[]);

int win;


int main(void) {
    initscr();              // start ncurses
    curs_set(FALSE);
    cbreak();
    keypad(stdscr, TRUE);
    noecho();
    nodelay(stdscr, TRUE);
    srand(time(0)); //seeds random function
    initializepit();        //create border around "snake pit"
    refresh();

    struct snake snake_array[((COLS * 2) + (LINES * 2))/2];
    int dir = rand() % 4;
    if(dir == 0){
        snake_array[0].current_direction = KEY_RIGHT;
    }
    else if(dir == 1){
        snake_array[0].current_direction = KEY_DOWN;
    }
    else if(dir == 2){
        snake_array[0].current_direction = KEY_LEFT;
    }
    else if(dir == 3){
        snake_array[0].current_direction = KEY_UP;
    }
    altmakesnake(snake_array[0].current_direction, snake_array);
    refresh();
    int gameend = 1;

    while (gameend) {
        //the game is played or something
        int input = getch();                // get user input

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
                setTrophy();
            }else{
                altmovesnake(input, snake_array);
            }         
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
                setTrophy();
            }else{
                altmovesnake(snake_array[0].current_direction, snake_array);
            }
        }
        int speed = 550000;
        if((550000-(10000 * snakestart)) < 0){
            speed = 10000;
        }
        else{
            speed = 550000 - (10000 * snakestart);
        }        
        usleep(speed); trophyExpiration--;//wait half a second at start, speeding up as the snake's length increases
        if(trophyExpiration == 0){
            mvprintw(trophyY, trophyX, " ");
            setTrophy();
        }
        refresh();
    }
    endwin();               // quit ncurses
    return 0;               // exit program
}


//Places a trophy, with a random value from 1 to 9, randomly in the pit
void setTrophy(){
    //get random value from 1 to 9
    trophyValue = (rand() % 9) + 1;
    trophyExpiration = ((rand() % 9) + 1)* 2;
    //print the value of the trophy on a random spot of the pit
    trophyX = (rand() % COLS -3) + 2;
    trophyY = (rand() % LINES -3) + 2;
    if(trophyX <= 0 || trophyX >= COLS -1 || trophyY <= 0 || trophyY >= LINES -1)
        mvprintw(2, 2, "Error, please try again"); 
    mvprintw(trophyY, trophyX, "%d", trophyValue);
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
    setTrophy(); //Should place a trophy in random spot of pit
}


void altmakesnake(int direction, struct snake array[]) {
    array[0].x = COLS/2; //head set to vertical center
    array[0].y = LINES/2; //head set to horizonal center
    array[0].current_direction = direction;  //head current direction set to passed direction

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
        array[i].current_direction = direction;
    }
    move(array[0].y, array[0].x);
    printw("@");

}

void altmovesnake(int direction, struct snake array[]) {
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

}

/*returns 0 if there is no character in the next space, and 1 if there is a character that isn't a trophy, and 2 if the character is a trophy
*/
int detectcollision(int direction, struct snake array[]){
    int testch;
    if(direction == KEY_LEFT){
        testch = mvinch(array[0].y, array[0].x - 1);
    }if(direction == KEY_RIGHT){
        testch = mvinch(array[0].y, array[0].x + 1);
    }if(direction == KEY_UP){
        testch = mvinch(array[0].y - 1, array[0].x);
    }if(direction == KEY_DOWN){
        testch = mvinch(array[0].y + 1, array[0].x);
    }
    if(testch == ' '){
        return 0;
    }else if (testch == '1' || testch == '2' || testch == '3' || testch == '4' || testch == '5' || testch == '6' || testch == '7' || testch == '8' || testch == '9'){
        return 2;
    }
    else{
        return 1;
    }
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
    for(int i=1;i<=trophyValue;i++){
        snakestart += 1; //add value to snake size
        array[snakestart - 1].x = array[snakestart - 2].x; array[snakestart - 1].y = array[snakestart - 2].y; //new tail given position of old tail
        array[snakestart - 1].current_direction = array[snakestart - 2].current_direction; //new tail given direction of old tail
        
        //If statement for direction of tail
        if(array[snakestart - 1].current_direction == KEY_LEFT){
            
            array[snakestart - 1].x += 1; //grow tail to the right
            mvprintw(array[snakestart - 1].y, array[snakestart - 1].x, "<"); //place left facing arrow in tail position
        }else if(array[snakestart - 1].current_direction == KEY_RIGHT){
            array[snakestart - 1].x -= 1; //grow tail to the left
            mvprintw(array[snakestart - 1].y, array[snakestart - 1].x, ">"); //place right facing arrow in tail position
        }else if(array[snakestart - 1].current_direction == KEY_UP){
            array[snakestart - 1].y += 1; //grow tail down
            mvprintw(array[snakestart - 1].y, array[snakestart - 1].x, "^"); //place up facing arrow in tail position
        }else if (array[snakestart - 1].current_direction== KEY_DOWN){
            array[snakestart - 1].y -= 1; //grow tail up
            mvprintw(array[snakestart - 1].y, array[snakestart - 1].x, "v"); //place down facing arrow in tail position
        }
    }
    move(array[0].y, array[0].x);
    refresh(); 
}
