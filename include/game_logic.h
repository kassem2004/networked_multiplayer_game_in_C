#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

void board_setup();
void add_to_board(int grid[10][10], char *placement);
void print_board(int board[10][10]);
int play_move(int grid[10][10], char *move);

#endif