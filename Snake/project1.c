#include <Windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#pragma warning(disable : 4996)

// colors
#define RED 12
#define BLUE 3
#define GREEN 10
#define YELLOW 14
#define GRAY 8
#define PINK 13
#define WHITE 15
// directions
#define UP 0
#define RIGHT 1
#define DOWN 2
#define LEFT 3
// snake constants
#define INITIAL_SNAKE_LENGTH 3
#define MINIMUM_SNAKE_LENGTH 2
#define MAX_LEN_SNAKES 30
#define COORDINATE 2
// general
#define BOARD_SIZE 40
#define NUMBER_OF_MOUSES 20
#define WAIT_TIME 100
// game stats
#define LOSE 0
#define CONTINUE 1
#define WIN 2
#define DRAW 3
// existence
#define EXISTS 1
#define DOESNT_EXIST 0
// board_characters
#define EMPTY '\0'
#define MOUSE 'm'
#define PLAYER1_SNAKE_HEAD '1'
#define PLAYER2_SNAKE_HEAD '2'
#define PLAYER1_SNAKE_BODY 'a'
#define PLAYER2_SNAKE_BODY 'b'
#define BERNARD_CLOCK 'c'
#define GOLDEN_STAR '*'
#define POISON 'x'
// Bernard, Poison and golden star
#define NUMBER_OF_POISONS 5
#define NUMBER_OF_GOLDEN_STARS 3
#define BERNARD_CLOCK_APPEARANCE_CHANCE 20
#define BERNARD_CLOCK_APPEARANCE_CHECK_PERIOD 2000
#define BERNARD_CLOCK_FROZEN_TIME 4000

CONSOLE_FONT_INFOEX former_cfi;
CONSOLE_CURSOR_INFO former_info;
COORD former_screen_size;

void reset_console() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleDisplayMode(consoleHandle, CONSOLE_WINDOWED_MODE, &former_screen_size);
    SetCurrentConsoleFontEx(consoleHandle, FALSE, &former_cfi);
    SetConsoleCursorInfo(consoleHandle, &former_info);
}

void hide_cursor(HANDLE consoleHandle) {
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
    former_screen_size.X = csbi.dwSize.X;
    former_screen_size.Y = csbi.dwSize.Y;
    COORD coord;
    SetConsoleDisplayMode(consoleHandle, CONSOLE_FULLSCREEN_MODE, &coord);
}

void init_screen() {
    HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
    set_full_screen_mode(consoleHandle);
    hide_cursor(consoleHandle);
    set_console_font_and_font_size(consoleHandle);
}

void wait_and_get_direction(int* player1_snake_direction, int* player2_snake_direction) {
    DWORD64 start_time, check_time;
    start_time = GetTickCount64();  // GetTickCount returns time in miliseconds
    check_time = start_time + WAIT_TIME;
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
        case 'w':
            if (*player1_snake_direction != DOWN) *player1_snake_direction = UP;
            break;
        case 'a':
            if (*player1_snake_direction != RIGHT) *player1_snake_direction = LEFT;
            break;
        case 's':
            if (*player1_snake_direction != UP) *player1_snake_direction = DOWN;
            break;
        case 'd':
            if (*player1_snake_direction != LEFT) *player1_snake_direction = RIGHT;
            break;
        default:
            break;
    }

    switch (player2_key_hit) {
        case 'i':
            if (*player2_snake_direction != DOWN) *player2_snake_direction = UP;
            break;
        case 'j':
            if (*player2_snake_direction != RIGHT) *player2_snake_direction = LEFT;
            break;
        case 'k':
            if (*player2_snake_direction != UP) *player2_snake_direction = DOWN;
            break;
        case 'l':
            if (*player2_snake_direction != LEFT) *player2_snake_direction = RIGHT;
            break;
        default:
            break;
    }
    return;
}

void draw_point(char point_content) {
    switch (point_content) {
        case PLAYER1_SNAKE_HEAD:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
            printf("@");
            break;
        case PLAYER2_SNAKE_HEAD:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
            printf("@");
            break;
        case PLAYER1_SNAKE_BODY:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), RED);
            printf("o");
            break;
        case PLAYER2_SNAKE_BODY:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BLUE);
            printf("o");
            break;
        case MOUSE:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GRAY);
            printf("m");
            break;
        case GOLDEN_STAR:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), YELLOW);
            printf("*");
            break;
        case POISON:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GREEN);
            printf("x");
            break;
        case BERNARD_CLOCK:
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), PINK);
            printf("c");
            break;
        default:
            printf(" ");
    }
}

void draw_horizontal_walls() {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
    for (int i = 0; i < BOARD_SIZE + 2; i++)
        printf("-");
    printf("\n");
}

void draw_board(char board_content[BOARD_SIZE][BOARD_SIZE]) {
    system("cls");
    draw_horizontal_walls();

    for (int i = 0; i < BOARD_SIZE; i++) {
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
        printf("|");  // vertical wall

        for (int j = 0; j < BOARD_SIZE; j++)
            draw_point(board_content[i][j]);

        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), WHITE);
        printf("|\n");  // vertical wall
    }

    draw_horizontal_walls();
}

// my functions
void put_initial_characters(char board_content[BOARD_SIZE][BOARD_SIZE], 
                           int snake1_coordinate[MAX_LEN_SNAKES][COORDINATE], 
                           int snake2_coordinate[MAX_LEN_SNAKES][COORDINATE]);

void put_snake(int snake_length, 
               int snake_coordinate[MAX_LEN_SNAKES][COORDINATE], 
               char board_content[BOARD_SIZE][BOARD_SIZE], int snake_mode);

void delete_snake_tail(int length, 
                       int snakes_coordinate[MAX_LEN_SNAKES][COORDINATE], 
                       char board_content[BOARD_SIZE][BOARD_SIZE]);

void switch_positions(int length, int snakes_coordinate[MAX_LEN_SNAKES][COORDINATE], 
                      int x, int y);

void finish_game(int first_mode, int second_mode);

void find_empty_position(int* x, int* y, char board_content[BOARD_SIZE][BOARD_SIZE]);

int update_snake1_coordinate(DWORD* bernard_apperance_time, 
                             DWORD* bernard_frozen_time, 
                             int* snake2_delay, int* bernard_watch, 
                             int* snake1_length, int* snake2_length, 
                             int snake1_coordinate[MAX_LEN_SNAKES][COORDINATE], 
                             int snake2_coordinate[MAX_LEN_SNAKES][COORDINATE], 
                             int player1_snake_direction, 
                             char board_content[BOARD_SIZE][BOARD_SIZE]);

int update_snake2_coordinate(DWORD* bernard_apperance_time, 
                             DWORD* bernard_frozen_time, 
                             int* snake1_delay, int* bernard_watch, 
                             int* snake1_length, int* snake2_length, 
                             int snake1_coordinate[MAX_LEN_SNAKES][COORDINATE], 
                             int snake2_coordinate[MAX_LEN_SNAKES][COORDINATE], 
                             int player2_snake_direction, 
                             char board_content[BOARD_SIZE][BOARD_SIZE]);

int main() {
    // declearing and initializing our variables
    char board_content[BOARD_SIZE][BOARD_SIZE] = {EMPTY};
    int snake1_length = INITIAL_SNAKE_LENGTH, 
        snake2_length = INITIAL_SNAKE_LENGTH,
        player1_snake_direction = UP, 
        player2_snake_direction = DOWN,
        snake1_coordinate[MAX_LEN_SNAKES][COORDINATE] = {EMPTY}, 
        snake2_coordinate[MAX_LEN_SNAKES][COORDINATE] = {EMPTY},
        mode1 = CONTINUE, mode2 = CONTINUE, 
        bernard_watch = DOESNT_EXIST, 
        snake1_delay = EMPTY, snake2_delay = EMPTY;

    DWORD bernard_appearance_time = GetTickCount64() + BERNARD_CLOCK_APPEARANCE_CHECK_PERIOD,
          bernard_frozen_time = GetTickCount64();

    srand(time(NULL));
    init_screen();
    put_initial_characters(board_content, snake1_coordinate, snake2_coordinate); 

    while (TRUE) {
        put_snake(snake1_length, snake1_coordinate, board_content, 1);
        put_snake(snake2_length, snake2_coordinate, board_content, 2);

        draw_board(board_content);

        wait_and_get_direction(&player1_snake_direction, &player2_snake_direction);

        if (bernard_frozen_time + snake1_delay < GetTickCount64()) {
            snake1_delay = EMPTY;
            mode1 = update_snake1_coordinate(
                &bernard_appearance_time, &bernard_frozen_time, 
                &snake2_delay, &bernard_watch, 
                &snake1_length, &snake2_length,
                snake1_coordinate, snake2_coordinate, 
                player1_snake_direction, board_content);
        }
        if (mode1 != CONTINUE)
            break;

        if (bernard_frozen_time + snake2_delay < GetTickCount64()) {
            snake2_delay = EMPTY;
            mode2 = update_snake2_coordinate(
                &bernard_appearance_time, &bernard_frozen_time, 
                &snake1_delay, &bernard_watch, 
                &snake1_length, &snake2_length,
                snake1_coordinate, snake2_coordinate, 
                player2_snake_direction, board_content);
        }
        if (mode2 != CONTINUE) 
            break;

        if (bernard_watch == DOESNT_EXIST && bernard_appearance_time <= GetTickCount64()) {
            int chance = rand() % 100;
            if (chance <= 20) {
                int x_for_watch, y_for_watch;
                find_empty_position(&x_for_watch, &y_for_watch, board_content);
                board_content[x_for_watch][y_for_watch] = BERNARD_CLOCK;
                bernard_watch = EXISTS;
            }
            bernard_appearance_time = GetTickCount64() + BERNARD_CLOCK_APPEARANCE_CHECK_PERIOD;
        }
    }
    // drawing our final board content
    put_snake(snake1_length, snake1_coordinate, board_content, 1);
    put_snake(snake2_length, snake2_coordinate, board_content, 2);
    draw_board(board_content);
    finish_game(mode1, mode2);
    getchar();
    return 0;
}

void finish_game(int first_mode, int second_mode) {
    reset_console();
    if (first_mode == WIN || second_mode == LOSE) 
        printf("player 1 has won");
    else if (first_mode == LOSE || second_mode == WIN) 
        printf("player 2 has won");
    else if (second_mode == DRAW || first_mode == DRAW) 
        printf("draw");
    else 
        printf("sorry something has gone wrong");
    return;
}

void switch_positions(int length, int snakes_coordinate[MAX_LEN_SNAKES][COORDINATE], 
                      int x, int y) {
    for (int i = length - 2; i >= 0; i--) {
        snakes_coordinate[i + 1][0] = snakes_coordinate[i][0];
        snakes_coordinate[i + 1][1] = snakes_coordinate[i][1];
    }
    // now we update head position
    snakes_coordinate[0][0] = x;
    snakes_coordinate[0][1] = y;
}

void find_empty_position(int* x, int* y, char board_content[BOARD_SIZE][BOARD_SIZE]) {
    do {
        *x = rand() % BOARD_SIZE;
        *y = rand() % BOARD_SIZE;
    } while (board_content[*x][*y] != EMPTY);
}

void put_initial_characters(char board_content[BOARD_SIZE][BOARD_SIZE], 
                            int snake1_coordinate[MAX_LEN_SNAKES][COORDINATE], 
                            int snake2_coordinate[MAX_LEN_SNAKES][COORDINATE]) {

    for (int i = 0; i < INITIAL_SNAKE_LENGTH; i++) {
        snake1_coordinate[i][0] = BOARD_SIZE - INITIAL_SNAKE_LENGTH + i;
        snake1_coordinate[i][1] = 0;
        snake2_coordinate[i][0] = INITIAL_SNAKE_LENGTH - i - 1;
        snake2_coordinate[i][1] = BOARD_SIZE - 1;
    }
    // we put them in case rand is in those positions
    put_snake(INITIAL_SNAKE_LENGTH, snake1_coordinate, board_content, 1);
    put_snake(INITIAL_SNAKE_LENGTH, snake2_coordinate, board_content, 2);

    int x, y;
    for (int i = 0; i < NUMBER_OF_MOUSES; i++) {
        find_empty_position(&x, &y, board_content);
        board_content[x][y] = MOUSE;
    }
    for (int i = 0; i < NUMBER_OF_POISONS; i++) {
        find_empty_position(&x, &y, board_content);
        board_content[x][y] = POISON;
    }
    for (int i = 0; i < NUMBER_OF_GOLDEN_STARS; i++) {
        find_empty_position(&x, &y, board_content);
        board_content[x][y] = GOLDEN_STAR;
    }
    return;
}

void put_snake(int snake_length, int snake_coordinate[MAX_LEN_SNAKES][COORDINATE], 
               char board_content[BOARD_SIZE][BOARD_SIZE], int snake_mode) {
    char head = (snake_mode == 1)? PLAYER1_SNAKE_HEAD : PLAYER2_SNAKE_HEAD;
    char body = (snake_mode == 1)? PLAYER1_SNAKE_BODY : PLAYER2_SNAKE_BODY;

    board_content[snake_coordinate[0][0]][snake_coordinate[0][1]] = head;
    for (int i = 1; i < snake_length; i++) 
        board_content[snake_coordinate[i][0]][snake_coordinate[i][1]] = body;
}

void delete_snake_tail(int length, int snakes_coordinate[MAX_LEN_SNAKES][COORDINATE], 
                       char board_content[BOARD_SIZE][BOARD_SIZE]) {
    // for deleting the previous snake's tail's coordinate from board content
    board_content[snakes_coordinate[length - 1][0]][snakes_coordinate[length - 1][1]] = EMPTY;
}

int update_snake1_coordinate(DWORD* bernard_apperance_time, DWORD* bernard_frozen_time, 
                             int* snake2_delay, int* bernard_watch, 
                             int* snake1_length, int* snake2_length,
                             int snake1_coordinate[MAX_LEN_SNAKES][COORDINATE], 
                             int snake2_coordinate[MAX_LEN_SNAKES][COORDINATE],
                             int player1_snake_direction, 
                             char board_content[BOARD_SIZE][BOARD_SIZE]) {

    int new_x, new_y;
    new_x = snake1_coordinate[0][0] + (player1_snake_direction == DOWN) - 
            (player1_snake_direction == UP);
    new_y = snake1_coordinate[0][1] + (player1_snake_direction == RIGHT) - 
            (player1_snake_direction == LEFT);

    // in case we hit the edges
    if (new_x >= BOARD_SIZE) 
        new_x -= BOARD_SIZE;
    if (new_x < 0)
        new_x += BOARD_SIZE;

    if (new_y >= BOARD_SIZE) 
        new_y -= BOARD_SIZE;
    if (new_y < 0)
        new_y += BOARD_SIZE;

    // now we check for different situation
    // but before that we have to delete snake's tail cause it's moving
    delete_snake_tail(*snake1_length, snake1_coordinate, board_content);

    if (board_content[new_x][new_y] == EMPTY ||
        ((new_x == snake2_coordinate[*snake2_length - 1][0] && 
          new_y == snake2_coordinate[*snake2_length - 1][1] && 
          *snake2_delay == EMPTY))) {
        switch_positions(*snake1_length, snake1_coordinate, new_x, new_y);

        return CONTINUE;
    }

    else if (board_content[new_x][new_y] == MOUSE) {
        if (*snake1_length < MAX_LEN_SNAKES)  // can't be more than our maximum
            (*snake1_length)++;

        switch_positions(*snake1_length, snake1_coordinate, new_x, new_y);

        // since we have lost one mouse, we have to replace it
        int new_x_for_mouse, new_y_for_mouse;
        find_empty_position(&new_x_for_mouse, &new_y_for_mouse, board_content);
        board_content[new_x_for_mouse][new_y_for_mouse] = MOUSE;

        return CONTINUE;
    }

    else if (board_content[new_x][new_y] == POISON) {
        (*snake1_length)--;
        if (*snake1_length < MINIMUM_SNAKE_LENGTH)  // if it's smaller than minimum
            return LOSE;

        delete_snake_tail(*snake1_length, snake1_coordinate, board_content);
        switch_positions(*snake1_length, snake1_coordinate, new_x, new_y);

        // since we have lost one poison we have to replace it
        int new_x_for_poison, new_y_for_poison;
        find_empty_position(&new_x_for_poison, &new_y_for_poison, board_content);
        board_content[new_x_for_poison][new_y_for_poison] = POISON;

        return CONTINUE;
    }

    else if (board_content[new_x][new_y] == GOLDEN_STAR) {
        delete_snake_tail(*snake2_length, snake2_coordinate, board_content);
        (*snake2_length)--;
        if (*snake2_length < MINIMUM_SNAKE_LENGTH)
            return WIN;

        switch_positions(*snake1_length, snake1_coordinate, new_x, new_y);

        // since we have lost one golden star we have to replace it
        int new_x_for_golden_star, new_y_for_golden_star;
        find_empty_position(&new_x_for_golden_star, &new_y_for_golden_star, board_content);
        board_content[new_x_for_golden_star][new_y_for_golden_star] = GOLDEN_STAR;

        return CONTINUE;
    }

    else if (board_content[new_x][new_y] == BERNARD_CLOCK) {
        *bernard_watch = DOESNT_EXIST;
        *bernard_frozen_time = GetTickCount64();
        *snake2_delay = BERNARD_CLOCK_FROZEN_TIME;
        *bernard_apperance_time = GetTickCount64() + BERNARD_CLOCK_APPEARANCE_CHECK_PERIOD;

        switch_positions(*snake1_length, snake1_coordinate, new_x, new_y);

        return CONTINUE;
    }

    else if (board_content[new_x][new_y] == PLAYER2_SNAKE_BODY || 
             board_content[new_x][new_y] == PLAYER1_SNAKE_BODY) {
        // we lose the game
        switch_positions(*snake1_length, snake1_coordinate, new_x, new_y);
        return LOSE;
    } 
    
    else if (board_content[new_x][new_y] == PLAYER2_SNAKE_HEAD) {
        switch_positions(*snake1_length, snake1_coordinate, new_x, new_y);
        // in this case we decide based on length
        if (*snake1_length > *snake2_length)
            return WIN;
        else if (*snake1_length < *snake2_length)
            return LOSE;
        else
            return DRAW;
    } 
    
    else if (*snake2_delay == BERNARD_CLOCK_FROZEN_TIME && 
             snake2_coordinate[*snake2_length - 1][0] == new_x && 
             snake2_coordinate[*snake2_length - 1][1] == new_y) {
        // if we hit the tail of other snake during bernard
        switch_positions(*snake1_length, snake1_coordinate, new_x, new_y);
        return LOSE;
    } 
    
    else {
        // something enexpected has happened
        return -1;
    }
}

int update_snake2_coordinate(DWORD* bernard_apperance_time, DWORD* bernard_frozen_time, 
                             int* snake1_delay, int* bernard_watch, 
                             int* snake1_length, int* snake2_length,
                             int snake1_coordinate[MAX_LEN_SNAKES][COORDINATE], 
                             int snake2_coordinate[MAX_LEN_SNAKES][COORDINATE],
                             int player2_snake_direction, 
                             char board_content[BOARD_SIZE][BOARD_SIZE]) {

    int new_x, new_y;
    new_x = snake2_coordinate[0][0] + (player2_snake_direction == DOWN) - 
            (player2_snake_direction == UP);
    new_y = snake2_coordinate[0][1] + (player2_snake_direction == RIGHT) - 
            (player2_snake_direction == LEFT);

    // in case we hit the edges
    if (new_x >= BOARD_SIZE)  
        new_x -= BOARD_SIZE;
    if (new_x < 0)
        new_x += BOARD_SIZE;

    if (new_y >= BOARD_SIZE) 
        new_y -= BOARD_SIZE;
    if (new_y < 0)
        new_y += BOARD_SIZE;

    // now we check for different situation but first we put them here
    put_snake(*snake1_length, snake1_coordinate, board_content, 1);
    put_snake(*snake2_length, snake2_coordinate, board_content, 2);
    delete_snake_tail(*snake2_length, snake2_coordinate, board_content);

    if (board_content[new_x][new_y] == EMPTY) {
        switch_positions(*snake2_length, snake2_coordinate, new_x, new_y);

        return CONTINUE;
    }

    else if (board_content[new_x][new_y] == MOUSE) {
        if (*snake2_length < MAX_LEN_SNAKES)  // can't be more than our maximum
            (*snake2_length)++;

        switch_positions(*snake2_length, snake2_coordinate, new_x, new_y);

        // since we have lost one mouse, we have to replace it
        int new_x_for_mouse, new_y_for_mouse;
        find_empty_position(&new_x_for_mouse, &new_y_for_mouse, board_content);
        board_content[new_x_for_mouse][new_y_for_mouse] = MOUSE;

        return CONTINUE;
    }

    else if (board_content[new_x][new_y] == POISON) {
        (*snake2_length)--;
        if (*snake2_length < MINIMUM_SNAKE_LENGTH)  // if it's smaller than minimum
            return LOSE;

        delete_snake_tail(*snake2_length, snake2_coordinate, board_content);
        switch_positions(*snake2_length, snake2_coordinate, new_x, new_y);

        // since we have lost one poison we have to replace it
        int new_x_for_poison, new_y_for_poison;
        find_empty_position(&new_x_for_poison, &new_y_for_poison, board_content);
        board_content[new_x_for_poison][new_y_for_poison] = POISON;

        return CONTINUE;
    }

    else if (board_content[new_x][new_y] == GOLDEN_STAR) {
        delete_snake_tail(*snake1_length, snake1_coordinate, board_content);
        (*snake1_length)--;
        if (*snake1_length < MINIMUM_SNAKE_LENGTH)  // if it's smaller than minimum
            return WIN;

        switch_positions(*snake2_length, snake2_coordinate, new_x, new_y);

        // since we have lost one golden star we have to replace it
        int new_x_for_golden_star, new_y_for_golden_star;
        find_empty_position(&new_x_for_golden_star, &new_y_for_golden_star, board_content);
        board_content[new_x_for_golden_star][new_y_for_golden_star] = GOLDEN_STAR;

        return CONTINUE;
    }

    else if (board_content[new_x][new_y] == BERNARD_CLOCK) {
        *bernard_watch = DOESNT_EXIST;
        *bernard_frozen_time = GetTickCount64();
        *snake1_delay = BERNARD_CLOCK_FROZEN_TIME;
        *bernard_apperance_time = GetTickCount64() + BERNARD_CLOCK_APPEARANCE_CHECK_PERIOD;
        // and then we switch
        switch_positions(*snake2_length, snake2_coordinate, new_x, new_y);

        return CONTINUE;
    }

    else if (board_content[new_x][new_y] == PLAYER2_SNAKE_BODY || 
             board_content[new_x][new_y] == PLAYER1_SNAKE_BODY) {
        switch_positions(*snake2_length, snake2_coordinate, new_x, new_y);
        return LOSE;
    } 

    else if (board_content[new_x][new_y] == PLAYER1_SNAKE_HEAD) {
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