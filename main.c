#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

//TODO Implement flags

struct box {
    char value;
    bool is_hidden;
    bool is_flagged;
};

time_t t;

int cols;
int rows;

int mines;

void initialize_settings(int init_width, int init_height, int init_mines) {
    cols = init_width;
    rows = init_height;
    
    mines = init_mines;
}

struct box **initialize_board() {

    struct box **game_board = malloc(rows * sizeof(struct box *));

    for(int i = 0; i < rows; i++) {
		game_board[i] = malloc(cols * sizeof(struct box));
    }

    if (mines > rows * cols) {
        //TODO too much mines
    }
    
    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < cols; y++) {
            struct box new_box = {'0', true, false};
            game_board[x][y] = new_box;
        }
    }
    
    int mines_counter = mines;

    while (mines_counter > 0) {
        int rand_x = rand() % cols;
        int rand_y = rand() % rows;

        while (game_board[rand_y][rand_x].value == 'X') {
            rand_x = rand() % cols;
            rand_y = rand() % rows;
        }

        game_board[rand_y][rand_x].value = 'X';

        for (int a = rand_y - 1; a < rand_y + 2; a++) {
            if (a < 0 || a > rows - 1) { continue; }
            for (int b = rand_x - 1; b < rand_x + 2; b++) {
                if (b < 0 || b > cols - 1) { continue; }
                int current_char_value = game_board[a][b].value;
                if (isdigit(current_char_value)) {
                    game_board[a][b].value = (char) (current_char_value + 1);
                }
            }
        }
        
        mines_counter--;
    }

    return game_board;
}

void print_raw_board(struct box **board) {
    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < cols; y++) {
            printf("%c ", board[x][y].value);
        }
        printf("\n");
    }
}

void print_game_board(struct box **board) {
    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < cols; y++) {
            if (board[x][y].is_flagged && board[x][y].is_hidden) {
                printf("F ");
            } else if (board[x][y].is_hidden) {
                printf("▀ ");
            } else  {
                printf("%c ", board[x][y].value);
            }
        }
        printf("\n");
    }
}

void rec_clear_numb(struct box **board, int input_row, int input_col) {
    for (int a = input_row - 1; a < input_row + 2; a++) {
        if (a < 0 || a > rows - 1) { continue; }
        for (int b = input_col - 1; b < input_col + 2; b++) {
            if (b < 0 || b > cols - 1) { continue; }
            if (a == input_row && b == input_col) { continue; }
            board[a][b].is_hidden = false;
        }
    }
}

void rec_clear_path(struct box **board, int input_row, int input_col) {
    for (int a = input_row - 1; a < input_row + 2; a++) {
        if (a < 0 || a > rows - 1) { continue; }
        for (int b = input_col - 1; b < input_col + 2; b++) {
            if (b < 0 || b > cols - 1) { continue; }
            if (a == input_row && b == input_col) { continue; }
            char current_char = board[a][b].value;
            if (current_char == '0' && board[a][b].is_hidden == true) {
                board[a][b].is_hidden = false;
                rec_clear_path(board, a, b);
                rec_clear_numb(board, a, b);
            }
        }
    }
}

bool check_mine_hit(struct box **board, int input_row, int input_col) {
    bool is_mine_hit = false;
    if (board[input_row][input_col].value == 'X') {
        is_mine_hit = true;
    }
    board[input_row][input_col].is_hidden = false;

    if (board[input_row][input_col].value == '0') {
        rec_clear_path(board, input_row, input_col);
    }
    
    return is_mine_hit;
}

bool check_box(struct box **board, int input_row, int input_col) {
    int current_box_mines = board[input_row][input_col].value - '0';
    int flag_count = 0;
    
    for (int a = input_row - 1; a < input_row + 2; a++) {
        if (a < 0 || a > rows - 1) { continue; }
        for (int b = input_col - 1; b < input_col + 2; b++) {
            if (b < 0 || b > cols - 1) { continue; }
            if (a == input_row && b == input_col) { continue; }
            
            char current_char = board[a][b].value;
            if (board[a][b].is_flagged) {
                flag_count++;
            }
        }
    }
    
    printf("MADE IT THIS FAR | %d | %d", current_box_mines, flag_count);
    if (current_box_mines == flag_count) {
        printf("EQUALS");
        for (int a = input_row - 1; a < input_row + 2; a++) {
            if (a < 0 || a > rows - 1) { continue; }
            for (int b = input_col - 1; b < input_col + 2; b++) {
                if (b < 0 || b > cols - 1) { continue; }
                if (a == input_row && b == input_col) { continue; }
                
                if (!board[a][b].is_flagged && board[a][b].is_hidden) {
                    if (check_mine_hit(board, a, b)) {
                        return true;
                    };
                    board[a][b].is_hidden = false;
                }
            }
        }
    }
    
    return false;
}

bool check_complete(struct box **board, int mines_count) {
    int hidden_and_flag_count = 0;
    for (int x = 0; x < rows; x++) {
        for (int y = 0; y < cols; y++) {
            if (board[x][y].is_hidden || board[x][y].is_flagged) {
                hidden_and_flag_count++;
            }
        }
    }
    return (hidden_and_flag_count == mines_count);
}

int main(void) {

    srand((unsigned) time(&t));
    
    int width = 5;
    int height = 10;
    int mines = 5;
    
    initialize_settings(width, height, mines);
    
    struct box **game_board = initialize_board();

    bool is_mine_hit = false;
    bool is_complete = false;
    
    int input_row, input_col;
    char input_type;
    
    while (!is_complete && !is_mine_hit) {
        printf("\n");
        //print_raw_board(game_board);
        printf("\n");
        print_game_board(game_board);

        printf("Flag type: 'F'\n");
        printf("Hit type: 'H'\n");
        printf("Check type: 'C'\n");
        printf("Enter row, column, and type as such: '0 0 H' > ");

        //TODO User Input Boundaries
        
        scanf("%d %d %c", &input_row, &input_col, &input_type);
        
        if (input_type == 'F') {
            game_board[input_row][input_col].is_flagged = true;
        } else if (input_type == 'C') {
            is_complete = check_box(game_board, input_row, input_col);
        } else if (input_type == 'H') {
            is_mine_hit = check_mine_hit(game_board, input_row, input_col);
            is_complete = check_complete(game_board, mines);
        }
        
    }

    print_game_board(game_board);

    if (is_mine_hit) {
        printf("You Lose!\n");
    } else {
        printf("You Win!\n");
    }
    
    return 0;
}
    