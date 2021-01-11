#include "sudoku.h"
#include <emscripten.h>
#include <emscripten/em_macros.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

bool row_validity(int *sudoku, int pos, int num) {
  int row = pos / SUDOKU_SIZE;
  for (int col = 0; col < SUDOKU_SIZE; col++) {
    if (num == sudoku[row * SUDOKU_SIZE + col]) {
      return false;
    }
  }
  return true;
}

bool col_validity(int *sudoku, int pos, int num) {
  int col = pos % SUDOKU_SIZE;
  for (int row = 0; row < SUDOKU_SIZE; row++) {
    if (num == sudoku[row * SUDOKU_SIZE + col]) {
      return false;
    }
  }
  return true;
}

bool grid_validity(int *sudoku, int pos, int num) {
  int row = pos / SUDOKU_SIZE;
  int col = pos % SUDOKU_SIZE;

  int grid_row = (row / SUDOKU_GRID_SIZE) * SUDOKU_GRID_SIZE;
  int grid_col = (col / SUDOKU_GRID_SIZE) * SUDOKU_GRID_SIZE;

  for (int r = 0; r < SUDOKU_GRID_SIZE; r++) {
    for (int c = 0; c < SUDOKU_GRID_SIZE; c++) {
      int check_pos = (grid_row + r) * SUDOKU_SIZE + (grid_col + c);
      if (num == sudoku[check_pos]) {
        return false;
      }
    }
  }
  return true;
}

bool pos_validity(int *sudoku, int pos, int num) {
  return (num > 0) ? (row_validity(sudoku, pos, num) &&
                      col_validity(sudoku, pos, num) &&
                      grid_validity(sudoku, pos, num))
                   : true;
}

int populate_entries(int *sudoku, int *entries) {
  int count = 0;
  for (int pos = 0; pos < SUDOKU_BOARD_SIZE; pos++) {
    if (sudoku[pos] == 0) {
      entries[count++] = pos;
    }
  }
  return count;
}

EMSCRIPTEN_KEEPALIVE
bool validate_board(int *sudoku) {
  for (int pos = 0; pos < SUDOKU_BOARD_SIZE; pos++) {
    int num = sudoku[pos];
    sudoku[pos] = 0;
    if (!pos_validity(sudoku, pos, num)) {
      sudoku[pos] = num;
      return false;
    }
    sudoku[pos] = num;
  }
  return true;
}

EMSCRIPTEN_KEEPALIVE
bool solve_board(int *sudoku) {
  int *free_positions = malloc(SUDOKU_BOARD_SIZE * sizeof(int));
  int count = populate_entries(sudoku, free_positions);
  int cur = 0;

  while (cur >= 0 && cur < count) {
    int pos = free_positions[cur];
    bool num_set = false;
    for (int num = sudoku[pos] + 1; num <= SUDOKU_SIZE; num++) {
      if (pos_validity(sudoku, pos, num)) {
        sudoku[pos] = num;
        num_set = true;
        break;
      }
    }
    if (num_set) {
      cur++;
    } else {
      sudoku[pos] = 0;
      cur--;
    }
  }

  free(free_positions);
  return (cur >= 0);
}

// weak generator, might produce an unsolvable board
EMSCRIPTEN_KEEPALIVE
void generate_board(int *sudoku) {
  srand(time(NULL));

  int slots = rand() % (SUDOKU_BOARD_SIZE / 4);
  for (int i = 0; i < slots; i++) {
    int pos = rand() % SUDOKU_BOARD_SIZE;
    int num = (rand() % SUDOKU_SIZE) + 1;
    if (sudoku[pos] == 0 && pos_validity(sudoku, pos, num)) {
      sudoku[pos] = num;
    }
  }
}
