/**
 * Snake game developed for CS 355
 */

#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>


int snakesize = 3;
int speed = 530000; //initial speed of snake is half a second
int trophyValue;
int trophyExpiration;
int trophyX;
int trophyY;
int win;

struct snake {
    int current_direction; 
    int x;
    int y;
};

void setTrophy();
void initializepit();
int detectcollision(int, struct snake[]);
void makesnake(int, struct snake[]);
void movesnake(int, struct snake[]);
void gameover();
void gamewin();
void growsnake(struct snake[]);




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
    //pick a random direction for the snake to start in
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
    makesnake(snake_array[0].current_direction, snake_array);
    refresh();
    int gameend = 1;

    while (gameend) {
        //the game is played or something
        int input = getch();                // get user input

        if (input == KEY_BACKSPACE) {
            gameend = 0;
        } else if (input == KEY_LEFT || input == KEY_RIGHT || input == KEY_DOWN || input == KEY_UP) {
            int testcoll = detectcollision(input, snake_array);
            if(testcoll == 1){//if the snake has run into a wall/itself
                gameover(); 
                refresh();               
                gameend = 0;
            }else if(testcoll == 2){//if the snake has run into a trophy
                movesnake(input, snake_array);
                growsnake(snake_array);
                if (snakesize >= win){
                    gameend = 0;
                }
                setTrophy();
            }else{
                movesnake(input, snake_array);
            }         
        } else {
            int testcoll = detectcollision(snake_array[0].current_direction, snake_array);
            if(testcoll == 1){//if the snake has run into a wall/itself
                gameover(); 
                refresh();               
                gameend = 0;
            }else if(testcoll == 2){//if the snake has run into a trophy
                movesnake(snake_array[0].current_direction, snake_array);
                growsnake(snake_array);
                if (snakesize >= win){
                    gamewin();
                    gameend = 0;
                }
                setTrophy();
            }else{
                movesnake(snake_array[0].current_direction, snake_array);
            }
        }
        
        if(speed > 10000){//prevents speed from becoming negative or too fast
            speed = 530000 - (10000 * snakesize);
        }       
        trophyExpiration -= speed;        
        usleep(speed);//wait half a second at start, speeding up as the snake's length increases
        if(trophyExpiration <= 0){
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
    trophyExpiration = ((rand() % 9) + 1) * 1000000;
    //trophyExpiration = 120 * 500000;          //<- uncomment this line if you want to make it fun

    //print the value of the trophy on a random spot of the pit
    trophyX = (rand() % (COLS -3)) + 2;
    trophyY = (rand() % (LINES -3)) + 2;
    while(mvinch(trophyX, trophyY) != ' '){
        trophyX = (rand() % (COLS -3)) + 2;
        trophyY = (rand() % (LINES -3)) + 2;
    }
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
    setTrophy(); //places a trophy in random spot of pit
}

//prints the initial snake to center of screen in given direction and populates the snake array
void makesnake(int direction, struct snake array[]) {
    array[0].x = COLS/2; //head set to vertical center
    array[0].y = LINES/2; //head set to horizonal center
    array[0].current_direction = direction;  //head current direction set to passed direction

    for (int i = 1; i < snakesize ; i++) {
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

//moves snake in the given direction 
void movesnake(int direction, struct snake array[]) {
    mvprintw(array[snakesize-1].y, array[snakesize-1].x, " ");
    //Each element's position is set to the position of the element before it
    for(int i = snakesize - 1; i > 0; i--){
        array[i].x = array[i-1].x; array[i].y = array[i-1].y;
        array[i].current_direction = array[i - 1].current_direction;
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

/*returns 0 if there is no character in the next space, and 1 if there is a character that isn't a trophy, 
and 2 if the character is a trophy */
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

//ends the game, printing text and outputting the score to the screen
void gameover(){
    mvprintw(LINES/2, (COLS/2)-10, "Game Over! Score: %d", snakesize);
    mvprintw(LINES/2 + 1, (COLS/2) - 11, "Press any key to exit");
    refresh();
    getchar();
}

//ends the game with a win message
void gamewin(){
    mvprintw(LINES/2 - 15, COLS/2, "You won! Press any key to exit");
    refresh();
    getchar();
}

//grow the snake in the opposite direction of the tail however many times as indicated by trophyValue
void growsnake(struct snake array[]){
    for(int i=1;i<=trophyValue;i++){
        snakesize += 1; //add value to snake size
        array[snakesize - 1].x = array[snakesize - 2].x; array[snakesize - 1].y = array[snakesize - 2].y; //new tail given position of old tail
        array[snakesize - 1].current_direction = array[snakesize - 2].current_direction; //new tail given direction of old tail
        
        //If statement for direction of tail
        if(array[snakesize - 1].current_direction == KEY_LEFT){
            
            array[snakesize - 1].x += 1; //grow tail to the right
            mvprintw(array[snakesize - 1].y, array[snakesize - 1].x, "<"); //place left facing arrow in tail position
        }else if(array[snakesize - 1].current_direction == KEY_RIGHT){
            array[snakesize - 1].x -= 1; //grow tail to the left
            mvprintw(array[snakesize - 1].y, array[snakesize - 1].x, ">"); //place right facing arrow in tail position
        }else if(array[snakesize - 1].current_direction == KEY_UP){
            array[snakesize - 1].y += 1; //grow tail down
            mvprintw(array[snakesize - 1].y, array[snakesize - 1].x, "^"); //place up facing arrow in tail position
        }else if (array[snakesize - 1].current_direction== KEY_DOWN){
            array[snakesize - 1].y -= 1; //grow tail up
            mvprintw(array[snakesize - 1].y, array[snakesize - 1].x, "v"); //place down facing arrow in tail position
        }
    }
    move(array[0].y, array[0].x);
    refresh(); 
}
