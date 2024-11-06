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

void trim_trailing_whitespace(char *str){
    int len = strlen(str);
    while(len > 0 && (str[len - 1] == '\n' || str[len - 1] == ' ' || str[len - 1] == '\t')) {
        str[len - 1] = '\0';
        len--;
    }
}

int validate_coordinate(int row, int col){
    if(row < 0 || row > 9 || col < 0 || col > 9){
        return 0;
    } else {
        return 1;
    }
}

int validate_piece(char *piece){
    if(strcmp(piece, "Submarine") == 0 || strcmp(piece, "Carrier") == 0 || strcmp(piece, "Cruiser") == 0|| strcmp(piece, "Battleship") == 0|| strcmp(piece, "Destroyer") == 0){
        return 1;
    } else {
        return 0;
    }
}

int validate_direction(char *direction){
    if(strcmp(direction, "Up") == 0 || strcmp(direction, "Down") == 0 || strcmp(direction, "Left") == 0 || strcmp(direction, "Right") == 0){
        return 1;
    } else {
        return 0;
    }
}

int check_piece(char *piece, char* placed_pieces[], int *p_count){
    for(int i = 0; i < *p_count; i++){
        if(strcmp(piece, placed_pieces[i]) == 0){
            return 0;
        }
    }
    //I WAS TRYING TO DO 'placed_pieces[*p_count] = piece' WHICH WAS THE PROBLEM!!! It was assiging the address of piece to that pointer in the array
    placed_pieces[*p_count] = strdup(piece);//SPENT SO LONG ON THIS, PLEASE DON"T FORGET!!!!
    return 1;
}

void print_board(int board[10][10]){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            printf("%d", board[i][j]);
        }
        printf("\n");
    }
}

int add_to_board(int grid[10][10], char *placement, char *placed_pieces[], int *p_count) {
    char *piece;       
    char *coordinate;   
    char *direction;
    char dir;
    int row, col;
    int moves = -1;
    char d[] = " ";
    piece = strtok(placement, d);
    coordinate = strtok(NULL, d);
    direction = strtok(NULL, d);

    if(piece == NULL || coordinate == NULL || direction == NULL){ //makes sure that there's 3 non-null inputs
        return 0;
    }

    trim_trailing_whitespace(direction);

    dir = direction[0];
    row = coordinate[0] - 'A';
    col = atoi(&coordinate[1]) - 1;

    if(validate_coordinate(row, col) == 0){
        return 0;
    }

    if(validate_piece(piece) == 0){
        return 0;
    }

    if(validate_direction(direction) == 0){
        return 0;
    }

    if(check_piece(piece, placed_pieces, p_count) == 0){
        return 0;
    }

    if(!strcmp(piece, "Carrier")){
        moves = 5;
    } else if(!strcmp(piece, "Battleship")){
        moves = 4;
    } else if(!strcmp(piece, "Destroyer")){
        moves = 2;
    } else if(!strcmp(piece, "Submarine") || !strcmp(piece, "Cruiser")){
        moves = 3;
    }

    switch(dir){
        case 'R':
            if((col + moves) > 10){ //boundary check
                return 0;
            }
            for(int i = 0; i < moves; i++){ //collision check
                if(grid[row][col + i] == 1){
                    return 0;
                }
            }
            for(int i = 0; i < moves; i++){
                grid[row][col + i] = 1;
            }
            break;
        case 'L':
            if((col - moves) < -1){
                return 0;
            }
            for(int i = 0; i < moves; i++){
                if(grid[row][col - i] == 1){
                    return 0;
                }
            }
            for(int i = 0; i < moves; i++){
                grid[row][col - i] = 1;
            }
            break;
        case 'U':
            if((row - moves) < -1){
                return 0;
            }
            for(int i = 0; i < moves; i++){
                if(grid[row - i][col] == 1){
                    return 0;
                }
            }
            for(int i = 0; i < moves; i++){
                grid[row - i][col] = 1;
            }
            break;
        case 'D':
            if((row + moves) > 10){
                return 0;
            }
            for(int i = 0; i < moves; i++){
                if(grid[row + i][col] == 1){
                    return 0;
                }
            }
            for(int i = 0; i < moves; i++){
                grid[row + i][col] = 1;
            }
            break;
    }
    print_board(grid);
    return 1;
}

int play_move(int board[10][10], char *move){
    int row, col;
    row = move[0] - 'A';
    if(strlen(move) == 3){
        col = atoi(&move[1]) - 1; //reminder, arrays decay to pointers of first element when passed into function
    } else {                        //but when you specificy a certain index, the value is passed in not a pointer
        col = move[1] - '1';
    }

    if(board[row][col] == 0){
        board[row][col] = 2; //2 indicates that this coordinate has been hit
        return 0;
    } else if(board[row][col] == 1){
        board[row][col] = 3; //3 indicates that this coordinate has been hit and ship was on it
        return 1;
    }
    return -1;
}