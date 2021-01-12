# Sudoku Solver (WebAssembly)

This is a simple Sudoku board generator and solver written in C and compiled to WebAssembly using Emscripten. 
The frontend is built with plain JavaScript, HTML, and CSS without any frameworks or libraries.

## Features

- Generates a partially filled Sudoku
- Validates the board according to Sudoku rules
- Solves the puzzle using a backtracking algorithm
- Locks both generated and user-filled cells before solving

## Tech Stack

- C (core logic)
- WebAssembly (compiled with Emscripten)
- JavaScript, HTML, and CSS (frontend)

## Build Instructions

To compile the C source into WebAssembly:

```bash
emcc src/sudoku.c -Os -s WASM=1 \
  -s EXPORTED_FUNCTIONS='["_solve_board", "_validate_board", "_generate_board", "_malloc", "_free"]' \
  -s EXPORTED_RUNTIME_METHODS='["ccall", "cwrap", "HEAP32"]' \
  -o public/solver.js
```

