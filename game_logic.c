#include "include/game_logic.h"
#include <stdio.h>
#include <stdlib.h>

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