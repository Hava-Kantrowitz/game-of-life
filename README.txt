John Conway's Game of Life, Systems Assignment 2, 1/31/19
Hava Kantrowitz

This program plays John Conway's game of life.

Starts with an array given by the user, and updates the array according to the following rules. 
Survival rule: If a cell is occupied and has 2-3 neighbors, it is occupied in the next generation.
Death rule: If a cell is occupied and has less than 2 or more than 3 neighbors, it is unoccupied in the next generation.
Birth rule: If a cell is unoccupied and has exactly 3 neighbors, it is occupied in the next generation. 

The game terminates upon one of 4 conditions. 
Dead board: Terminates when all cells are unoccupied. 
Repeated generation: Terminates when the current generation is the same as the previous generation.
Repeated grandfather generation: Terminates when the current generation is the same as the grandfather generation.
User generations: Terminates when the number of user-specified generations is reached. This case is only hit if the earlier 3 generations haven't been triggered. 

Requires 4 arguments, ability for 2 optional arguments. Less than 4 or greater than 7 arguments will result in an error. 

Required values:
NR: number of rows in array, an unsigned int
NC: number of columns in array, an unsigned int
gens: number of maximum generations to go through, must be greater than 0
inputfile: file that contains initial generation of array, must be less than 100 characters long

Optional values:
print: 'y' to print the board after each generation, 'n' otherwise
pause: 'y' to wait for user input to continue after each generation, 'n' otherwise
Print and pause default to 'n' if not provided by the user

When using keystroke to move forward during a pause, use the enter key.
Initial user input is considered generation zero. 

All values outside of the board size specified by the user default to unoccupied. Any generation that would extend off the board is truncated; only values on the given board are calculated and communicated to the user. 

Starter code provide by Professor Smith. 

The biggest issue I had with this assignment was understanding how to adjust where the pointers were pointing in the next generation of the array. It was easier to grasp when I stopped thinking of them as old array, new array, grandfather array, and instead thought of them as arrays A, B, and C. 
