#include <stdlib.h>
#include <stdio.h>

int width;
int height;

int mines;

void initialize_settings(int init_width, int init_height, int init_mines) {
    width = init_width;
    height = init_height;

    printf("%d | %d", width, height);
    
    mines = init_mines;
}

int **initialize_board() {
    
    int **gameBoard;
	
	gameBoard = (int **)malloc(sizeof(int *)*width);
	
	for (int i = 0; i < height; i++) {
		gameBoard[i] = (int *)malloc(sizeof(int)*height);
	}

    for (int x = 0; x < height; x++) {
        for (int y = 0; y < width; y++) {
            gameBoard[x][y] = 0;
        }
    }

    return gameBoard;
}

void print_board(int **board) {
    for (int x = 0; x < height; x++) {
        for (int y = 0; y < width; y++) {
            printf("%d", board[x][y]);
        }
        printf("\n");
    }
}
/*
int **initialize_board() {

    int **gameBoard;
	
	gameBoard = (int **)malloc(sizeof(int *)*9);
	
	for (int i = 0; i < 5; i++) {
		gameBoard[i] = (int *)malloc(sizeof(int)*5);
	}

    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 9; y++) {
            gameBoard[x][y] = 0;
        }
    }

    return gameBoard;
}

void print_board(int **board) {
    for (int x = 0; x < 5; x++) {
        for (int y = 0; y < 9; y++) {
            printf("%d", board[x][y]);
        }
        printf("\n");
    }
}
*/