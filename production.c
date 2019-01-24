/*
 * production.c
 *
 *  Created on: Jan 24, 2019
 *      Author: Hava Kantrowitz
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "production.h"
#include <math.h>

/**
 * Reads input and produces the game board for Life
 * @param argc WHAT IT IS
 * @param argv WHAT IT IS
 * @return true if production is completed successfully
 */
bool production(int argc, char* argv[])
{
	bool results = false;
	bool done = false;
	int nRows=-1;
	int nCols = -1;
	int gens = 0; //number of generations to play
	//etc.
	//get the NR NC gens input [print] [pause], Usage as needed.

	// Must first check if there are at least 4 arguments on the command line


	// Next, check if print and pause are on the command line
	if(argc<7)
	{//must be that pause is not included, so give it a default value

	}
	else
	{
		char* ptr=0;
		long nr_l = strtol(argv[1],&ptr,10);//get the NR
		nRows = (int)nr_l;
		if(nRows<1)
		{
			printf("Usage: Rows should be greater than 0, received %d.\n",nRows);
			done = true;
		}
	}
	return results;

}
/**
 * Plays one generation of life
 * @param nr number of rows on board
 * @param nc number of columns on board
 * @param Old original array
 * @param New changed array
 * @return void
 */
void PlayOne (unsigned int nr, unsigned int nc, char Old[][nc], char New[][nc])
{
	int nRows = nr;
	int nCols = nc;
	for(int row = 0;row < nRows;row++)
	{
		for(int col=0; col < nCols; col++)
		{
			*( ((char*) New) + (row * nCols) + col) = *( ((char*) Old) + (row * nCols) + col);
		}
	}
}

/**
 * Determines if a cell in an array is occupied
 * @param row the row the cell is in
 * @param col the column the cell is in
 * @param cell the array in which the cell occurs
 * @param nRow the number of rows in the array
 * @param nCol the number of columns in the array
 * @return 1 if the cell is occupied, 0 if the cell is not occupied
 * 			If the cell is unoccupied, it will have a 'o' in it
 * 			If the cell is occupied, it will have a 'x' in it
 * 			It is assumed that there will be no values besides 'o' and 'x'
 * 				within the given cell
 * 			All values outside the range of the array are considered unoccupied
 */
int occupied(int row, int col, char cell[][col], int nRow, int nCol){
	int occupied = 1;
	if (row > nRow || row < 0){
		occupied = 0;
	}
	else if (col > nCol || col < 0){
		occupied = 0;
	}
	else if (cell[row][col] == 'o'){
		occupied = 0;
	}

	return occupied;

}

/**
 * Compares two arrays
 * @param nRows number of rows in arrays
 * @param nCols number of columns in arrays
 * @param array1 the first array to compare
 * @param array2 the second array to compare
 * @return 1 if the arrays are the same, 0 if the arrays are different
 */
int arrayComparison(int nRows, int nCols, char array1[][nCols], char array2[][nCols]){
	int same = 1;
	for(int row = 0; row < nRows; row++){
		for (int col = 0; col < nCols; col++){
			if (occupied(row,col,array1,nRows,nCols) == occupied(row,col,array2,nRows,nCols)){
				same = 0;
			}
		}
	}

	return same;
}

/**
 * Determines the number of neighbors a cell has
 * @param row the row the cell is in
 * @param col the column the cell is in
 * @param array the array in which the cell occurs
 * @param totalR total rows of array
 * @param totalC total columns of array
 * @return number of neighbors the cell has
 */
int numNeighbors(int row, int col, char array[][col], int totalR, int totalC){
	int neighbors = 0;
	//check upper left corner
	if (occupied(row-1, col-1, array, totalR, totalC)){
		neighbors++;
	}
	//check direct above
	if (occupied(row-1, col, array, totalR, totalC)){
		neighbors++;
	}
	//check upper right corner
	if (occupied(row-1, col+1, array, totalR, totalC)){
		neighbors++;
	}
	//check direct left
	if (occupied(row, col-1, array, totalR, totalC)){
		neighbors++;
	}
	//check direct right
	if (occupied(row, col+1, array, totalR, totalC)){
		neighbors++;
	}
	//check lower left corner
	if (occupied(row+1, col-1, array, totalR, totalC)){
		neighbors++;
	}
	//check direct lower
	if(occupied(row+1, col, array, totalR, totalC)){
		neighbors++;
	}
	//check lower right corner
	if (occupied(row+1, col+1, array, totalR, totalC)){
		neighbors++;
	}
	return neighbors;
}

/**
 * determines if ending conditions are met
 * @param nRows number of rows in array
 * @param nCols number of columns in array
 * @param previousArray the array from the generation before
 * @param nowArray the array created in the current generation
 * @return 1 if terminating conditions are met, 0 if terminating conditions aren't met
 */
int endGame(int nRows, int nCols, char previousArray[][nCols], char nowArray[][nCols]){
	//code
}

