#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>

#pragma warning(disable:4996)

//colors
#define RED 12
#define BLUE 3
#define GREEN 10
#define YELLOW 14
#define GRAY 8
#define PINK 13
#define WHITE 15
#define WAIT_TIME_MILI_SEC 100
//directions		
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
// general
#define BOARD_SIZE 40
#define INITIAL_SNAKE_LENGTH 3
#define MINIMUM_SNAKE_LENGTH 2
#define MAX_LEN_SNAKES 30
#define NUMBER_OF_MOUSES 20
#define COORDINATE 2
#define CONTINUE 1
#define LOSE 0
#define WIN 2
#define DRAW 3
#define EXISTS 1
#define DOESNT_EXIST 0
//board_characters
#define EMPTY '\0'
#define MOUSE 'm'
#define PLAYER1_SNAKE_HEAD '1'
#define PLAYER2_SNAKE_HEAD '2'
#define PLAYER1_SNAKE_BODY 'a'
#define PLAYER2_SNAKE_BODY 'b'
//Bernard, Poison and golden star
#define BERNARD_CLOCK 'c' //on the board character
#define GOLDEN_STAR '*' //on the board character
#define POISON 'x' //on the board character
#define NUMBER_OF_POISONS 5
#define NUMBER_OF_GOLDEN_STARS 3
#define BERNARD_CLOCK_APPEARANCE_CHANCE_PERCENT 20
#define BERNARD_CLOCK_APPEARANCE_CHECK_PERIOD_MILI_SEC 2000
#define BERNARD_CLOCK_FROZEN_TIME_MILI_SEC 4000

CONSOLE_FONT_INFOEX former_cfi;
CONSOLE_CURSOR_INFO former_info;
COORD former_screen_size;

void reset_console() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleDisplayMode(consoleHandle, CONSOLE_WINDOWED_MODE, &former_screen_size);
    SetCurrentConsoleFontEx(consoleHandle, FALSE, &former_cfi);
    SetConsoleCursorInfo(consoleHandle, &former_info);
}

void hidecursor(HANDLE consoleHandle) {
    GetConsoleCursorInfo(consoleHandle, &former_info);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    SetConsoleCursorInfo(consoleHandle, &info);
}

void set_console_font_and_font_size(HANDLE consoleHandle) {
    former_cfi.cbSize = sizeof(former_cfi);
    GetCurrentConsoleFontEx(consoleHandle, FALSE, &former_cfi);
    CONSOLE_FONT_INFOEX cfi;
    cfi.cbSize = sizeof(cfi);
    cfi.nFont = 0;
    cfi.dwFontSize.X = 20;
    cfi.dwFontSize.Y = 20;
    cfi.FontFamily = FF_DONTCARE;
    cfi.FontWeight = FW_NORMAL;
    wcscpy(cfi.FaceName, L"Courier");
    SetCurrentConsoleFontEx(consoleHandle, FALSE, &cfi);
}

void set_full_screen_mode(HANDLE consoleHandle) {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    former_screen_size.X = csbi.dwSize.X; former_screen_size.Y = csbi.dwSize.Y;
    COORD coord;
    SetConsoleDisplayMode(consoleHandle, CONSOLE_FULLSCREEN_MODE, &coord);
}

void init_screen() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    set_full_screen_mode(consoleHandle);
    hidecursor(consoleHandle);
    set_console_font_and_font_size(consoleHandle);

}

void wait_and_get_direction(int* player1_snake_direction, int* player2_snake_direction) {
    DWORD64 start_time, check_time;
    start_time = GetTickCount64();
    check_time = start_time + WAIT_TIME_MILI_SEC; //GetTickCount returns time in miliseconds
    char key = 0;
    char player1_key_hit = 0;
    char player2_key_hit = 0;

    while (check_time > GetTickCount64()) {
        if (_kbhit()) {
            key = _getch();
            if (key == 0)
                key = _getch();
            if (key == 'w' || key == 'a' || key == 's' || key == 'd')
                player1_key_hit = key;
            if (key == 'i' || key == 'j' || key == 'k' || key == 'l')
                player2_key_hit = key;
        }
    }

    switch (player1_key_hit) {
    case 'w': if (*player1_snake_direction != DOWN) *player1_snake_direction = UP; break;
    case 'a': if (*player1_snake_direction != RIGHT) *player1_snake_direction = LEFT; break;
    case 's': if (*player1_snake_direction != UP) *player1_snake_direction = DOWN; break;
    case 'd': if (*player1_snake_direction != LEFT) *player1_snake_direction = RIGHT; break;
    default: break;
    }

    switch (player2_key_hit) {
    case 'i': if (*player2_snake_direction != DOWN) *player2_snake_direction = UP; break;
    case 'j': if (*player2_snake_direction != RIGHT) *player2_snake_direction = LEFT; break;
    case 'k': if (*player2_snake_direction != UP) *player2_snake_direction = DOWN; break;
    case 'l': if (*player2_snake_direction != LEFT) *player2_snake_direction = RIGHT; break;
    default: break;
    }
    return;
}

void draw_point(char point_content) {
    switch (point_content) {
    case PLAYER1_SNAKE_HEAD: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED); printf("@"); break;
    case PLAYER2_SNAKE_HEAD: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);  printf("@"); break;
    case PLAYER1_SNAKE_BODY: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);  printf("o"); break;
    case PLAYER2_SNAKE_BODY: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);  printf("o"); break;
    case MOUSE: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GRAY); printf("m"); break;
    case GOLDEN_STAR: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW); printf("*"); break;
    case POISON: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN); printf("x"); break;
    case BERNARD_CLOCK: SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PINK); printf("c"); break;
    default: printf(" ");
    }
}

void draw_horizonatal_walls() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
    for (int i = 0; i < BOARD_SIZE + 2; ++i)
        printf("-");
    printf("\n");
}

void draw_board(char board_content[BOARD_SIZE][BOARD_SIZE]) {
    system("cls");
    draw_horizonatal_walls();
    for (int i = 0; i < BOARD_SIZE; i++) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
        printf("|"); // vertical wall 
        for (int j = 0; j < BOARD_SIZE; j++)
            draw_point(board_content[i][j]);
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
        printf("|\n"); // vertical wall
    }
    draw_horizonatal_walls();
}



// my functions
void put_initial_characters(char board_content[BOARD_SIZE][BOARD_SIZE], int snake1_coordinate[MAX_LEN_SNAKES][COORDINATE], int snake2_coordinate[MAX_LEN_SNAKES][COORDINATE]);
void put_first_snake(int snake1_length, int snake1_coordinate[MAX_LEN_SNAKES][COORDINATE], char board_content[BOARD_SIZE][BOARD_SIZE]);
void put_second_snake(int snake2_length, int snake2_coordinate[MAX_LEN_SNAKES][COORDINATE], char board_content[BOARD_SIZE][BOARD_SIZE]);
void delete_snake_tail(int length, int snakes_coordinate[MAX_LEN_SNAKES][COORDINATE], char board_content[BOARD_SIZE][BOARD_SIZE]);
void switch_positions(int length, int snakes_coordinate[MAX_LEN_SNAKES][COORDINATE], int x, int y);
void finish_game(int first_mode, int second_mode);
void find_empty_position(int* x, int* y, char board_content[BOARD_SIZE][BOARD_SIZE]);
int update_snake1_coordinate(DWORD* bernard_apperance_time, DWORD* bernard_frozen_time, int* snake2_delay, int* bernard_watch, int* snake1_length, int* snake2_length, int snake1_coordinate[MAX_LEN_SNAKES][COORDINATE], int snake2_coordinate[MAX_LEN_SNAKES][COORDINATE], int player1_snake_direction, char board_content[BOARD_SIZE][BOARD_SIZE]);
int update_snake2_coordinate(DWORD* bernard_apperance_time, DWORD* bernard_frozen_time, int* snake1_delay, int* bernard_watch, int* snake1_length, int* snake2_length, int snake1_coordinate[MAX_LEN_SNAKES][COORDINATE], int snake2_coordinate[MAX_LEN_SNAKES][COORDINATE], int player2_snake_direction, char board_content[BOARD_SIZE][BOARD_SIZE]);


int main() {
    //declearing and initializing our variables
    char board_content[BOARD_SIZE][BOARD_SIZE] = { EMPTY };
    int snake1_length = INITIAL_SNAKE_LENGTH, snake2_length = INITIAL_SNAKE_LENGTH,
        player1_snake_direction = UP, player2_snake_direction = DOWN,
        snake1_coordinate[MAX_LEN_SNAKES][COORDINATE] = { EMPTY }, snake2_coordinate[MAX_LEN_SNAKES][COORDINATE] = { EMPTY },
        mode1 = CONTINUE, mode2 = CONTINUE, bernard_watch = DOESNT_EXIST, snake1_delay = EMPTY, snake2_delay = EMPTY;
    DWORD  bernard_apperance_time = GetTickCount64() + BERNARD_CLOCK_APPEARANCE_CHECK_PERIOD_MILI_SEC,
        bernard_frozen_time = GetTickCount64();

    srand(time(NULL)); // generating seed for rand function
    init_screen(); //call this to make window adjustments
    put_initial_characters(board_content, snake1_coordinate, snake2_coordinate); // to initializing our board
    
    while (TRUE) {
        //these two put the snakes in the board content
        put_first_snake(snake1_length, snake1_coordinate, board_content);
        put_second_snake(snake2_length, snake2_coordinate, board_content);

        //drawing our board content
        draw_board(board_content);

        //getting direction
        wait_and_get_direction(&player1_snake_direction, &player2_snake_direction);

        //then updating the positions and checking the results
        if (bernard_frozen_time + snake1_delay < GetTickCount64()) {
            snake1_delay = EMPTY;
            mode1 = update_snake1_coordinate(&bernard_apperance_time, &bernard_frozen_time, &snake2_delay, &bernard_watch, &snake1_length, &snake2_length,
                snake1_coordinate, snake2_coordinate, player1_snake_direction, board_content);
        }
        if (mode1 != CONTINUE)
            break;

        if (bernard_frozen_time + snake2_delay < GetTickCount64()) {
            snake2_delay = EMPTY;
            mode2 = update_snake2_coordinate(&bernard_apperance_time, &bernard_frozen_time, &snake1_delay, &bernard_watch, &snake1_length, &snake2_length,
                snake1_coordinate, snake2_coordinate, player2_snake_direction, board_content);
        }
        if (mode2 != CONTINUE)  //if any of them fails to meet the needs, we end the game
            break;

        if (bernard_watch == DOESNT_EXIST && bernard_apperance_time <= GetTickCount64()) {
            //if it doesn't exist for 20 percent we have to show it
            int chance = rand() % 100;
            if (chance <= BERNARD_CLOCK_APPEARANCE_CHANCE_PERCENT) {
                int x_for_watch, y_for_watch;
                find_empty_position(&x_for_watch, &y_for_watch, board_content);
                board_content[x_for_watch][y_for_watch] = BERNARD_CLOCK;
                bernard_watch = EXISTS;
            }
            bernard_apperance_time = GetTickCount64() + BERNARD_CLOCK_APPEARANCE_CHECK_PERIOD_MILI_SEC;
        }
    } 
    //drawing our final board content
    put_first_snake(snake1_length, snake1_coordinate, board_content);
    put_second_snake(snake2_length, snake2_coordinate, board_content);
    draw_board(board_content);
    //just to see the final board
    printf("press enter to see the resutlt");
    getchar();
    //finishing our game
    finish_game(mode1, mode2);
    //just to see the results
    getchar();
    return 0;
}

void finish_game(int first_mode, int second_mode) {
    //for checking endgame
    //input:
        //first_mode and second_mode
    //no output
    reset_console();
    if (first_mode == WIN || second_mode == LOSE) {
        printf("player 1 has won");
    }
    else if (first_mode == LOSE || second_mode == WIN) {
        printf("player 2 has won");
    }
    else if (second_mode == DRAW || first_mode == DRAW) {
        printf("draw");
    }
    else {
        printf("sorry something wrong has happened");
    }
    return;
}

void switch_positions(int length, int snakes_coordinate[MAX_LEN_SNAKES][COORDINATE], int x, int y) {
    // we switch coordinates of both snakes in this function
    //input:
        //length, x, y
        //snake_coordinate
    //no output
    //we switch all but head, the last one's value will be deleted
    for (int i = length - 2; i >= 0; i--) {
        snakes_coordinate[i + 1][0] = snakes_coordinate[i][0];
        snakes_coordinate[i + 1][1] = snakes_coordinate[i][1];
    }
    // now we update head position
    snakes_coordinate[0][0] = x;
    snakes_coordinate[0][1] = y;
    return;
}

void put_initial_characters(char board_content[BOARD_SIZE][BOARD_SIZE], int snake1_coordinate[MAX_LEN_SNAKES][COORDINATE], int snake2_coordinate[MAX_LEN_SNAKES][COORDINATE]) {
    //this function initializes our board content
    //input:
        //snake1_coordinate and snake2_coordinate  
        //board_content
    //no output

    //initializing first snake, each row is for a coordniate of each element of snake
     //and it's consist of x-coordinate and y-coordinate
    for (int i = 0; i < INITIAL_SNAKE_LENGTH; i++) {
        snake1_coordinate[i][0] = BOARD_SIZE - INITIAL_SNAKE_LENGTH + i;
        snake1_coordinate[i][1] = 0;
    }

    //initializing second snake, just like the first one
    for (int i = 0; i < INITIAL_SNAKE_LENGTH; i++) {
        snake2_coordinate[i][0] = INITIAL_SNAKE_LENGTH - i - 1;
        snake2_coordinate[i][1] = BOARD_SIZE - 1;
    }
    //we put them in case rand is in those positions
    put_first_snake(INITIAL_SNAKE_LENGTH, snake1_coordinate, board_content);
    put_second_snake(INITIAL_SNAKE_LENGTH, snake2_coordinate, board_content);

    //put random mouses
    int x, y;
    for (int i = 0; i < NUMBER_OF_MOUSES; i++) {
        find_empty_position(&x, &y, board_content);
        board_content[x][y] = MOUSE;
    }

    //put random posions
    for (int i = 0; i < NUMBER_OF_POISONS; i++) {
        find_empty_position(&x, &y, board_content);
        board_content[x][y] = POISON;
    }

    //put random golden stars
    for (int i = 0; i < NUMBER_OF_GOLDEN_STARS; i++) {
        find_empty_position(&x, &y, board_content);
        board_content[x][y] = GOLDEN_STAR;
    }
    return;
}

void put_first_snake(int snake1_length, int snake1_coordinate[MAX_LEN_SNAKES][COORDINATE], char board_content[BOARD_SIZE][BOARD_SIZE]) {
    //this fuction is dedicated to putting the coordinate's of first snake in board content
    //input:
        //board_content
        //snake1_coordinate and snake1_length
    //no output 

    board_content[snake1_coordinate[0][0]][snake1_coordinate[0][1]] = PLAYER1_SNAKE_HEAD;
    for (int i = 1; i < snake1_length; i++) {
        board_content[snake1_coordinate[i][0]][snake1_coordinate[i][1]] = PLAYER1_SNAKE_BODY;
    }
    return;
}

void put_second_snake(int snake2_length, int snake2_coordinate[MAX_LEN_SNAKES][COORDINATE], char board_content[BOARD_SIZE][BOARD_SIZE]) {
    //this fuction is dedicated to putting the coordinate's of second snake in board content
    //input:
        //board_content
        //snake2_coordinate and snake2_length
    //no output 

    board_content[snake2_coordinate[0][0]][snake2_coordinate[0][1]] = PLAYER2_SNAKE_HEAD;
    for (int i = 1; i < snake2_length; i++) {
        board_content[snake2_coordinate[i][0]][snake2_coordinate[i][1]] = PLAYER2_SNAKE_BODY;
    }
    return;
}

void delete_snake_tail(int length, int snakes_coordinate[MAX_LEN_SNAKES][COORDINATE], char board_content[BOARD_SIZE][BOARD_SIZE]) {
    //for deleting the previous snake's tail's coordinate from board content 
    //input:
        //board_content
        //snake2_coordinate and snake2_length
    //no output

    board_content[snakes_coordinate[length - 1][0]][snakes_coordinate[length - 1][1]] = EMPTY;
    return;
}

void find_empty_position(int* x, int* y, char board_content[BOARD_SIZE][BOARD_SIZE]) {
    //this function is to find empty positions for replacing
    //input:
        //*x and *y
        //board_content
    //no output
    do {
        *x = rand() % BOARD_SIZE;
        *y = rand() % BOARD_SIZE;
    } while (board_content[*x][*y] != EMPTY);
    return;
}

int update_snake1_coordinate(DWORD* bernard_apperance_time, DWORD* bernard_frozen_time, int* snake2_delay, int* bernard_watch, int* snake1_length, int* snake2_length,
    int snake1_coordinate[MAX_LEN_SNAKES][COORDINATE], int snake2_coordinate[MAX_LEN_SNAKES][COORDINATE],
    int player1_snake_direction, char board_content[BOARD_SIZE][BOARD_SIZE]) {
    // this function is used for updating poistion of first snake and also checking for different events that mat occur
    // input:
        // *snake1_length and *snake2_length and *snake2_delay
        // *bernard_frozen_time
        // player2_snake_direction
        // snake1_coordinate and snake2_coordinate
        // board_content
    // output:
          // is either win or lose or continue

        //we update the head position first in another variable
    int new_x, new_y;
    new_x = snake1_coordinate[0][0] + (player1_snake_direction == DOWN) - (player1_snake_direction == UP);
    new_y = snake1_coordinate[0][1] + (player1_snake_direction == RIGHT) - (player1_snake_direction == LEFT);

    if (new_x >= BOARD_SIZE) // in case we hit the edges
        new_x -= BOARD_SIZE;
    if (new_x < 0)
        new_x += BOARD_SIZE;

    if (new_y >= BOARD_SIZE) // like new_x
        new_y -= BOARD_SIZE;
    if (new_y < 0)
        new_y += BOARD_SIZE;

    //now we check for different situation
    // but before that we have to delete snake's tail cause it's moving
    delete_snake_tail(*snake1_length, snake1_coordinate, board_content);

    if (board_content[new_x][new_y] == EMPTY ||
        ((new_x == snake2_coordinate[*snake2_length - 1][0] && new_y == snake2_coordinate[*snake2_length - 1][1] && *snake2_delay == EMPTY))) {
        //if new position for head is empty we just simply update the body one by one (each by the privious one)
        // the second condition is for when we hit the end of the other snake during game and it's actually empty
        //cause we haven't updated it yet

        // we just switch
        switch_positions(*snake1_length, snake1_coordinate, new_x, new_y);

        // game is not over
        return CONTINUE;
    }

    else if (board_content[new_x][new_y] == MOUSE) {
        // if new position for head is mouse we update the body one by one (each by the privious one)
        // difference here is that we updat one more element since it's bigger now

        if (*snake1_length < MAX_LEN_SNAKES) // can't be more than our maximum
            (*snake1_length)++;

        //and then we switch
        switch_positions(*snake1_length, snake1_coordinate, new_x, new_y);

        // since we have lost one mouse, we have to replace it
        int new_x_for_mouse, new_y_for_mouse;
        find_empty_position(&new_x_for_mouse, &new_y_for_mouse, board_content);
        board_content[new_x_for_mouse][new_y_for_mouse] = MOUSE;

        // game is not overs
        return CONTINUE;
    }

    else if (board_content[new_x][new_y] == POISON) {
        //if we hit poison we have reduce the size (by putting zero in last coordinate)
        //and also check if it's smaller than two

        //reduce the size
        (*snake1_length)--;
        if (*snake1_length < MINIMUM_SNAKE_LENGTH) //if it's smaller than minimum
            return LOSE;
        // now we gotta delete previous snake's tail
        delete_snake_tail(*snake1_length, snake1_coordinate, board_content);

        //and then we switch
        switch_positions(*snake1_length, snake1_coordinate, new_x, new_y);

        // since we have lost one poison we have to replace it
        int new_x_for_poison, new_y_for_poison;
        find_empty_position(&new_x_for_poison, &new_y_for_poison, board_content);
        board_content[new_x_for_poison][new_y_for_poison] = POISON;

        //not over yet
        return CONTINUE;
    }

    else if (board_content[new_x][new_y] == GOLDEN_STAR) {
        //if we hit golden star we have reduce the size of the other one  and also check if it's smaller than two

        //delete other snake's tail
        delete_snake_tail(*snake2_length, snake2_coordinate, board_content);
        //reduce other snake's size
        (*snake2_length)--;
        if (*snake2_length < MINIMUM_SNAKE_LENGTH) //if it's smaller than minimum
            return WIN;

        //and then we switch
        switch_positions(*snake1_length, snake1_coordinate, new_x, new_y);

        // since we have lost one golden star we have to replace it
        int new_x_for_golden_star, new_y_for_golden_star;
        find_empty_position(&new_x_for_golden_star, &new_y_for_golden_star, board_content);
        board_content[new_x_for_golden_star][new_y_for_golden_star] = GOLDEN_STAR;

        //not over yet
        return CONTINUE;
    }

    else if (board_content[new_x][new_y] == BERNARD_CLOCK) {
        // if we hit bernard watch
        *bernard_watch = DOESNT_EXIST;
        *bernard_frozen_time = GetTickCount64();
        *snake2_delay = BERNARD_CLOCK_FROZEN_TIME_MILI_SEC;
        *bernard_apperance_time = GetTickCount64() + BERNARD_CLOCK_APPEARANCE_CHECK_PERIOD_MILI_SEC;
        //and then we switch
        switch_positions(*snake1_length, snake1_coordinate, new_x, new_y);

        //not over yet
        return CONTINUE;
    }

    else if (board_content[new_x][new_y] == PLAYER2_SNAKE_BODY || board_content[new_x][new_y] == PLAYER1_SNAKE_BODY) {
        // we lose the game
        switch_positions(*snake1_length, snake1_coordinate, new_x, new_y);
        return LOSE;
    }
    else if (board_content[new_x][new_y] == PLAYER2_SNAKE_HEAD) {
        //if we hit the other one's head
        switch_positions(*snake1_length, snake1_coordinate, new_x, new_y);
        if (*snake1_length > *snake2_length)
            return WIN;
        else if (*snake1_length < *snake2_length)
            return LOSE;
        else
            return DRAW;
    }
    else if (*snake2_delay == BERNARD_CLOCK_FROZEN_TIME_MILI_SEC && snake2_coordinate[*snake2_length - 1][0] == new_x && snake2_coordinate[*snake2_length - 1][1] == new_y) {
        //if we hit the tail of other snake during bernard
        switch_positions(*snake1_length, snake1_coordinate, new_x, new_y);
        return LOSE;
    }
     else {
        // something enexpected has happened
         return -1;                                               
     }                                                                                             
}

int update_snake2_coordinate(DWORD* bernard_apperance_time, DWORD* bernard_frozen_time, int* snake1_delay, int* bernard_watch, int* snake1_length, int* snake2_length,
    int snake1_coordinate[MAX_LEN_SNAKES][COORDINATE], int snake2_coordinate[MAX_LEN_SNAKES][COORDINATE],
    int player2_snake_direction, char board_content[BOARD_SIZE][BOARD_SIZE]) {
    // this function is used for updating poistion of second snake and also checking for different events that mat occur
    // input:
        // *snake1_length and *snake2_length and *snake1_delay
        // *bernard_frozen_time
        // player2_snake_direction
        // snake1_coordinate and snake2_coordinate
        // board_content
    // output:
             // is either win or lose or draw or continue

            //we update the head position first in another variable
    int new_x, new_y;
    new_x = snake2_coordinate[0][0] + (player2_snake_direction == DOWN) - (player2_snake_direction == UP);
    new_y = snake2_coordinate[0][1] + (player2_snake_direction == RIGHT) - (player2_snake_direction == LEFT);

    if (new_x >= BOARD_SIZE) // in case we hit the edges
        new_x -= BOARD_SIZE;
    if (new_x < 0)
        new_x += BOARD_SIZE;

    if (new_y >= BOARD_SIZE) // like new_x
        new_y -= BOARD_SIZE;
    if (new_y < 0)
        new_y += BOARD_SIZE;

    //now we check for different situation but first we put them here
    put_first_snake(*snake1_length, snake1_coordinate, board_content);
    put_second_snake(*snake2_length, snake2_coordinate, board_content);
    // but before that we have to delete our snake's tail
    delete_snake_tail(*snake2_length, snake2_coordinate, board_content);

    if (board_content[new_x][new_y] == EMPTY) {
        //if new position for head is empty we just simply update the body one by one (each by the privious one)
        //we just switch
        switch_positions(*snake2_length, snake2_coordinate, new_x, new_y);

        // game is not over
        return CONTINUE;
    }

    else if (board_content[new_x][new_y] == MOUSE) {
        // if new position for head is mouse we update the body one by one (each by the privious one)
        // difference here is that we updat one more element since it's bigger now

        if (*snake2_length < MAX_LEN_SNAKES) // can't be more than our maximum
            (*snake2_length)++;

        //and then we switch
        switch_positions(*snake2_length, snake2_coordinate, new_x, new_y);

        // since we have lost one mouse, we have to replace it
        int new_x_for_mouse, new_y_for_mouse;
        find_empty_position(&new_x_for_mouse, &new_y_for_mouse, board_content);
        board_content[new_x_for_mouse][new_y_for_mouse] = MOUSE;

        // game is not overs
        return CONTINUE;
    }

    else if (board_content[new_x][new_y] == POISON) {
        //if we hit poison we have reduce the size (by putting zero in last coordinate) 
        //and also check if it's smaller than two;

        //reduce size
        (*snake2_length)--;
        if (*snake2_length < MINIMUM_SNAKE_LENGTH) //if it's smaller than minimum
            return LOSE;

        //we gotta delete the current snake's tail again
        delete_snake_tail(*snake2_length, snake2_coordinate, board_content);
        //and then we switch
        switch_positions(*snake2_length, snake2_coordinate, new_x, new_y);

        // since we have lost one poison we have to replace it
        int new_x_for_poison, new_y_for_poison;
        find_empty_position(&new_x_for_poison, &new_y_for_poison, board_content);
        board_content[new_x_for_poison][new_y_for_poison] = POISON;

        //not over yet
        return CONTINUE;
    }

    else if (board_content[new_x][new_y] == GOLDEN_STAR) {
        //if we hit golden star we have reduce the size of the other one (by putting zero in last coordinate)
        // and also check if it's smaller than two

        //we gotta delete the other snake's tail
        delete_snake_tail(*snake1_length, snake1_coordinate, board_content);

        //reduce other snake's size
        (*snake1_length)--;
        if (*snake1_length < MINIMUM_SNAKE_LENGTH) //if it's smaller than minimum
            return WIN;

        //and then we switch
        switch_positions(*snake2_length, snake2_coordinate, new_x, new_y);

        // since we have lost one golden star we have to replace it
        int new_x_for_golden_star, new_y_for_golden_star;
        find_empty_position(&new_x_for_golden_star, &new_y_for_golden_star, board_content);
        board_content[new_x_for_golden_star][new_y_for_golden_star] = GOLDEN_STAR;

        //not over yet
        return CONTINUE;
    }

    else if (board_content[new_x][new_y] == BERNARD_CLOCK) {
        // if we hit bernard watch
        *bernard_watch = DOESNT_EXIST;
        *bernard_frozen_time = GetTickCount64(); 
        *snake1_delay = BERNARD_CLOCK_FROZEN_TIME_MILI_SEC;
        *bernard_apperance_time = GetTickCount64() + BERNARD_CLOCK_APPEARANCE_CHECK_PERIOD_MILI_SEC;
        //and then we switch
        switch_positions(*snake2_length, snake2_coordinate, new_x, new_y);

        //not over yet
        return CONTINUE;
    }

    else if (board_content[new_x][new_y] == PLAYER2_SNAKE_BODY || board_content[new_x][new_y] == PLAYER1_SNAKE_BODY) {
        // we lose the game
        switch_positions(*snake2_length, snake2_coordinate, new_x, new_y);
        return LOSE;
    }
    else if (board_content[new_x][new_y] == PLAYER1_SNAKE_HEAD) {
        // we need to check lenghts
        switch_positions(*snake2_length, snake2_coordinate, new_x, new_y);
        if (*snake2_length > *snake1_length)
            return WIN;
        else if (*snake2_length < *snake1_length)
            return LOSE;
        else
            return DRAW;
    }
    else {
         // something enexpected has happened
         return -1;
    }
}