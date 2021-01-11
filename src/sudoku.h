#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdbool.h>

#define SUDOKU_SIZE 9
#define SUDOKU_GRID_SIZE 3
#define SUDOKU_BOARD_SIZE 81

bool validate_board(int *sudoku);
bool solve_board(int *sudoku);
void generate_board(int *sudoku);

#endif // !SUDOKU_H
