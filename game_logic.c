#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/game_logic.h"

void board_setup() {
    printf("=== Game Setup Instructions ===\n\n");
    printf("Welcome! Set up your board by placing your ships on a 10x10 grid.\n");
    printf("Each player has the following ships:\n");
    printf("1. Carrier - 5 cells long\n");
    printf("2. Battleship - 4 cells long\n");
    printf("3. Cruiser - 3 cells long\n");
    printf("4. Submarine - 3 cells long\n");
    printf("5. Destroyer - 2 cells long\n\n");

    printf("To place each ship, enter:\n");
    printf("    <Piece> <Starting Coordinate> <Direction>\n");
    printf("For example:\n");
    printf("    Carrier A1 Right\n\n");

    printf("Coordinate System:\n");
    printf("    Rows: A-J\n");
    printf("    Columns: 1-10\n");
    printf("Example Coordinates:\n");
    printf("    A1 is the top-left corner\n");
    printf("    J10 is the bottom-right corner\n\n");

    printf("Place each ship within boundaries, and avoid overlaps.\n\n");
    printf("Your board:\n\n");
    printf("   1  2  3  4  5  6  7  8  9  10\n");
    for (char row = 'A'; row <= 'J'; row++) {
        printf("%c  ", row);  //row label
        for (int col = 1; col <= 10; col++) {
            printf("~  ");
        }
        printf("\n");
    }
} 

void add_to_board(int grid[10][10], char *placement) {
    char *piece;       
    char *coordinate;   
    char *direction;
    char dir;
    int row, col;
    int moves = -1;
    char d[] = " ";
    printf("%s", placement);
    piece = strtok(placement, d);
    coordinate = strtok(NULL, d);
    direction = strtok(NULL, d);


    dir = direction[0];
    row = coordinate[0] - 'A';
    col = atoi(&coordinate[1]) - 1;

    if(!strcmp(piece, "Carrier")){
        moves = 5;
    } else if(!strcmp(piece, "Battleship")){
        moves = 4;
    } else if(!strcmp(piece, "Destroyer")){
        moves = 2;
    } else if(!strcmp(piece, "Submarine") || !strcmp(piece, "Cruiser")){
        moves = 3;
    }

    if (row < 0 || row >= 10 || col < 0 || col >= 10) {
        fprintf(stderr, "Coordinates out of bounds: %s\n", coordinate);
        return;
    }

    switch(dir){
        case 'R':
            for(int i = 0; i < moves; i++){
                grid[row][col + i] = 1;
            }
            break;
        case 'L':
            for(int i = 0; i < moves; i++){
                grid[row][col - i] = 1;
            }
            break;
        case 'U':
            for(int i = 0; i < moves; i++){
                grid[row + i][col] = 1;
            }
            break;
        case 'D':
            for(int i = 0; i < moves; i++){
                grid[row - i][col] = 1;
            }
            break;
    }
}

void print_board(int board[10][10]){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            printf("%d", board[i][j]);
        }
    }
}