#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

void trim_trailing_whitespace(char *str);
int validate_coordinate(int row, int col);
int validate_piece(char *piece);
int validate_direction(char *direction);
void board_setup();
int add_to_board(int grid[10][10], char *placement);
void print_board(int board[10][10]);
int play_move(int grid[10][10], char *move);

#endif