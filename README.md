# Parallel N-Queens Solver

A parallelized solution to the N-Queens problem using multithreading in C. The program calculates the number of ways to place N queens on an NxN chessboard so no two queens threaten each other.

## How It Works

- Each thread starts with a different initial column configuration.
- The solution count is protected by a mutex to ensure thread safety.

## Requirements

- C compiler (e.g., `gcc`)
- POSIX Threads library (`pthread.h`)

## Usage

1. Compile the code:

   ```bash
   gcc -o nqueens nqueens.c -pthread
   
2. Run the program with the number of queens as an argument:
    
    ```bash
    ./nqueens <number_of_queens>
