/*
 * production.c
 *
 *  Created on: Jan 30, 2019
 *      Author: Hava Kantrowitz
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "production.h"
#include <math.h>
#include <string.h>

/**
 * @param argc Number of words on the command line
 * @param argv Array of pointers to character strings representing the words on the command line.
 * @return true if game of life was successfully played
 *         false if user entered bad input
 */
bool production(int argc, char* argv[])
{
	bool results = false; //results of game, initialized to false
	bool ok2 = false; //for opening file, initialized to false
	bool done = false; //for completion of production code, initialized to false
	int nRows=-1;//number of rows in game board, initialized to -1
	int nCols = -1;//number of columns in game board, initialized to -1
	int gens = 0; //number of generations to play, initialized to 0
	int howManyLinesInFile = 0;//number of rows in starting configuration, initialized to 0
	int maximumWidth = 0;//number of columns in starting configuration, initialized to 0
	char filename[100];//creates array of characters to hold filename
	for(int i= 0; i<100; i++)//loops through filename array, initializing all values to null
	{
		filename[i]='\0';
	}
	char print = 'n';//character to determine if there is a print, initialized to no
	char pause = 'n';//character to determine if there is a pause, initialized to no

	//get the NR NC gens input [print] [pause], Usage as needed.
	if(argc<5)//not all mandatory args provided
	{
		usage();
		done=true;//set done to true, finishing production
	}
	else if(argc > 7){//too many args provided
		usage();
		done = true;//set done to true, finishing production
	}
	else if (argc == 7)//if there are enough arguments on the command line, there must be pause and print
	{
		pause = argv[6][0];//record the value in the "pause" column in the pause variable
	}
	if (!done && (argc >= 6))//if there are at least 6 args on command line, there must be print
	{
		print = argv[5][0];//record value in "print" column in the print variable
	}

	if(!done)//must be greater than or equal to 5, so get the mandatory vals
	{

		//get the number of rows from the command line
		char* ptr=0;//pointer to character, initialized to 0
		long nr_l = strtol(argv[1],&ptr,10);//get the NR as a decimal number, store in long datatype
		nRows = (int)nr_l;//set the number of rows equal to the NR input, cast NR to int
		if(nRows<1)//If there aren't enough rows to play the game, let user know and end production
		{
			printf("Usage: Rows should be greater than 0, received %d.\n",nRows);
			done = true;
		}

		//get the number of columns from the command line
		char* ptr1 = 0;//pointer to character, initialized to 0
		long nc_l = strtol(argv[2],&ptr1,10);//get the NR as a decimal number, store in long datatype
		nCols = (int)nc_l;//set the number of rows equal to the NC input, cast NC to int
		if(nRows<1)//If there aren't enough columns to play the game, let user know and end production
		{
			printf("Usage: Columns should be greater than 0, received %d.\n",nCols);
			done = true;
		}

		//get the number of generations from the command line
		char* ptr2 = 0;//pointer to character, initialized to 0
		long gen_l = strtol(argv[3],&ptr2,10);//get the generation as a decimal number, store in long datatype
		gens = (int)gen_l;//set the number of gens equal to the generation input, cast gen to int
		if(gens<1)//If there aren't enough generations to play the game, let user know and end production
		{
			printf("Usage: Generations should be greater than 0, received %d.\n",nCols);
			done = true;
		}

		strcpy(filename, argv[4]);//makes a copy of the file on the command line
		//Let's read the input file
		FILE* fp = fopen(filename, "r"); //we try to read it
		if (fp != false)
		{//it opened, yay!
			printf("Opened %s.\n",filename);
			ok2 = true;
			//Read the data
			char oRow[100];
			//Find out lines and total width
			bool doneReadingFile = false;
			while(!doneReadingFile)//can file is not done being read
			{
				oRow[0]='\0';
				fscanf(fp, "%s", oRow);
				if(strlen(oRow)!=0)//there was something there
				{
					howManyLinesInFile++;//increment the number of lines in the file

					//if the length of the row is longer than the current max width
					//set it as the new max width
					if (maximumWidth < strlen(oRow)){
						maximumWidth = strlen(oRow);
					}
				}
				else//when there's no lines left, close the file
				{
					doneReadingFile = true;
					fclose(fp);
				}
			}

		}
		else//If cannot find/read file, let user know and end production
		{
			puts("Cannot find that file");
			done = true;
		}

		//If the number of columns and/or rows in the file is greater than the gameboard array, let user know and end production
		if (maximumWidth > nCols || howManyLinesInFile > nRows){
			usage();
			done = true; //set done to true, finishing production
		}

		if(!done)//if production hasn't ended, fill arrays
		{
			char A[nRows][nCols];//old array
			char B[nRows][nCols];//new array
			char C[nRows][nCols];//spare array
			char* old_p=&A[0][0];//pointer to old array
			char* new_p=&B[0][0];//pointer to new array
			char* other_p=&C[0][0];//pointer to spare array

			//loop through rows and columns of A, B, C, initializing them all to dead cells
			for(int row = 0; row< nRows; row++)
			{
				for(int col = 0; col<nCols; col++)
				{
					*(old_p + (row * nCols) + col)='o';
					*(new_p + (row * nCols) + col)='o';
					*(other_p + (row * nCols) + col)='o';
				}
			}
			FILE* fp = fopen(filename, "r");//we read it before, we expect we can read it again
			readFileIntoArray(nRows, nCols, howManyLinesInFile,  maximumWidth, old_p, fp);//read file into old array

			//Let user know the initial, user-given pattern
			puts("Initial pattern is:");
			printThis(nRows, nCols, old_p);
			//determine the total number of generations ran before completion
			int howManyGens = generate(gens,  nRows,  nCols,  old_p, new_p, other_p, print, pause);
			howManyGens--;//decrement gens ended on to adjust for starting on the 0th gen
			printf("Ran %d generations\n", howManyGens);//let user know the number of gens
		}

	}//not done
	results = !done;
	return results;

}

/**
 * PlayOne carries out one generation
 * @param unsigned int nr, the number of rows in the petri dish
 * @param unsigned int nc, the number of columns in the petri dish
 * @param char* Old, the location of the upper left of starting petri dish
 * @param char* New, the location of the upper left of the ending petri dish
 * @return There is none, because results are in New array
 */
void PlayOne (unsigned int nr, unsigned int nc, char* Old, char* New)
{
	int nRows = nr;//store number of rows
	int nCols = nc;//store number of columns
	//loop over all locations in the new board
	//figure out whether occupied or not
	for(int row=0;row<nRows;row++)
	{
		for(int col=0; col<nCols; col++)
		{
			//There are three rules,
			// use occupied in old
			//use neighbors in old
			bool occupied = true;
			occupied = 	(getLetter(row,col,nCols, Old)=='x');//if the letter in column and row is x, set occupied to true

			//death rule
			if(occupied
					&&((HowManyNeighbors(row, col, nRows, nCols, Old)<2)
							|| (HowManyNeighbors(row, col, nRows, nCols, Old)>3)))
			{
				*(New + (row * nCols) + col)='o';//use pointer arithmetic to set location to unoccupied
			}
			//Survival rule
			else if(occupied)
			{
				*(New + (row * nCols) + col)='x';//use pointer arithmetic to set location to occupied
			}
			//Birth rule
			else if(HowManyNeighbors(row, col, nRows, nCols, Old)==3)
			{
				*(New + (row * nCols) + col)='x';//use pointer arithmetic to set location to occupied

			}
			//If no rules apply, space is unoccupied
			else
			{
				*(New + (row * nCols) + col)='o';//use pointer arithmetic to set location to unoccupied
			}
		}
	}
}

/**
 * Lets user know what inputs are necessary to successfully run the game
 * @return void, prints user instructions to console
 */
void usage(void)
{
	puts("Usage: HW2 NR NC gens input [print] [pause]");
}

/**
 * gets letter in a specific row and column of an array
 * @param row: row of the cell under determinations
 * @param col: column of the cell under considerations
 * @param nCols: total number of columns in array
 * @param Old: array the cell is in
 * @return the character currently in the given cell
 * 				either 'x' or 'o'
 */
char getLetter(int row, int col, int nCols, char* Old)
{
	return *(Old+ (row*nCols)+col);
}

/**
 * determines the number of neighbors of a specific cell
 * @param row: row of the cell under determinations
 * @param col: column of the cell under considerations
 * @param nRows: total number of rows in array
 * @param nCols: total number of columns in array
 * @param Old: array the cell is in
 * @return the number of neighbors of the cell, a value 0 to 8 inclusive
 */
int HowManyNeighbors(int row, int col, int nRows, int nCols, char* Old)
{
	int howManyN = 0;//initialize number of neighbors to 0
	//there could be as many as 8 neighbors
	//cells on an edge or corner have fewer neighbors
	//we will search for neighbors clockwise from NorthWest
	if (row>0)//search only if row numbers north are on board
	{
		if(col>0)//search only if column numbers north are on board
		{
			if(getLetter(row-1, col-1,nCols, Old)=='x')//If there is an occupied cell to NW, increment neighbors by 1
			{
				howManyN++;
			}
		}
		if(getLetter(row-1, col,nCols, Old)=='x')//If there is an occupied cell to N, increment neighbors by 1
		{
			howManyN++;
		}
		if(col<(nCols-1))
		{
			if(getLetter(row-1, col+1,nCols, Old)=='x')//If there is an occupied cell to NE, increment neighbors by 1
			{
				howManyN++;
			}
		}
	}
	if(col>0)
	{
		if(getLetter(row, col-1,nCols, Old)=='x')//W
		{
			howManyN++;
		}
	}
	if(col<(nCols-1))//search only if column number to east is on board
	{
		if(getLetter(row, col+1,nCols, Old)=='x')//If there is an occupied cell to E, increment neighbors by 1
		{
			howManyN++;
		}
	}
	if(row<nRows-1)//search only if row number south is on board
	{
		if(col>0)//search only if column to west is on board
		{
			if(getLetter(row+1, col-1,nCols, Old)=='x')//If there is an occupied cell to SW, increment neighbors by 1
			{
				howManyN++;
			}
		}
		if(getLetter(row+1, col,nCols, Old)=='x')//If there is an occupied cell to S, increment neighbors by 1
		{
			howManyN++;
		}
		if(col<(nCols-1))//search only if column to east is on board
		{
			if(getLetter(row+1, col+1,nCols, Old)=='x')//If there is an occupied cell to NW, increment neighbors by 1
			{
				howManyN++;
			}
		}
	}

	return howManyN;//return total number of neighbors
}

/**
 * reads input file into array, initializing each cell to unoccupied
 * @param nRows: number of rows on gameboard
 * @param nCols: number of columns on gameboard
 * @param howManyLinesInFile: number of rows in file
 * @param maximumWidth: number of columns in file
 * @param ar_p: pointer to character array that file will be read into
 * @param fp: pointer to file that will be read into array
 * @return void, properly dimensions file array and reads all rows in columns into array
 */
void readFileIntoArray(int nRows, int nCols, int howManyLinesInFile, int maximumWidth, char* ar_p, FILE* fp)
{
	for(int row = 0; row< nRows; row++)//loops through all rows and columns on gameboard, initializing all cells to o
	{
		for(int col = 0; col<nCols; col++)
		{
			*(ar_p+(row*nCols)+col) = 'o';

		}
	}

	//loops through all rows and columns in file, inserting the proper value into each cell
	for(int frow=0; frow< howManyLinesInFile; frow++)
	{
		char fromFile[maximumWidth];//creates character array with number of columns of file

		for(int i = 0; i<maximumWidth; i++)
		{
			fromFile[i]='o';
		}
		fscanf(fp, "%s", fromFile);
		for(int fcol=0; fcol<maximumWidth; fcol++)
		{
			if(fromFile[fcol]=='x')
			{
				int targetRow = frow+(nRows-howManyLinesInFile)/2;//centers rows of file in gameboard
				int targetCol = fcol+(nCols-maximumWidth)/2;//centers columns of file in gameboard
				*(ar_p+(targetRow*nCols)+targetCol) = 'x';
			}
		}
	}
}

/**
 * generates the values for the next generation of play and determines when the game ends
 * @param gens: number of max generations given by user
 * @param nRows: total number of rows in array
 * @param nCols: total number of columns in array
 * @param old_p: pointer to array that holds previous generation's values, the array under scrutiny
 * @param new_p: pointer to array the holds current generation's values
 * @param other_p: pointer to array that holds two previous generation's values
 * @param print: 'y' if user wants it to print generation, 'n' if user doesn't or didn't give input
 * @param pause: 'y' if user wants to pause between generations, 'n' if user doesn't or didn't give input
 * @return the number of generations played in total
 */
int generate(int gens,  int nRows,  int nCols,  char* old_p, char* new_p, char* other_p, char print, char pause)
{
	int g = 0;//the number of generations played, initialized to 0
	bool allOrganismsDead = false;//for keeping track if all cells are dead, initialized to false
	bool patternRepeated = false;//for keeping track if the array pattern has repeated, initialized to false
	bool done = false;//for keeping track if production is finished, initialized to false
	bool firstTime = true;//for keeping track if this is the first time a new generation has been attempted, initialized to true
	char* spare_p; //creates spare pointer

	for(int gensDone = 0; !done && (gensDone<gens); gensDone++)//Loops through each generation until max number of generations is reached
	{
		if(!anyX(old_p, nRows, nCols))//if no cells have an x, all organisms are dead, end game and let user know
		{
			allOrganismsDead =  true;
			done = true;
			puts("All organisms dead.");
			puts("Final pattern is:");
			printThis(nRows, nCols, old_p);
		}
		PlayOne(nRows, nCols, old_p, new_p);//play a new generation and increase the generation number
		g++;
		//if the current array is the same as the previous array and game hasn't already ended, end game and let user know
		if (!done && sameContent(old_p, new_p, nRows, nCols))
		{
			patternRepeated = true;
			done = true;
			puts("Pattern repeated in one generation.");
			puts("Final pattern is:");
			printThis(nRows, nCols, old_p);
		}
		if(firstTime)//if it's the first time the game has been played, change to false
		{
			firstTime = false;
		}
		else//all other cases
		{
			//if the current array is same as 2 previous array and game hasn't ended, end game and let user know
			if (!done && sameContent(new_p, other_p, nRows, nCols))
			{
				patternRepeated = true;
				puts("Pattern repeated after two generations.");
				puts("Final pattern is:");
				printThis(nRows, nCols, other_p);
				done= true;

			}
		}
		if(!done)//if game hasn't ended, keep going
		{
			if(print=='y')//if user wants it to print, print the generation
			{
				printf("New generation. Generation number %d.\n",gensDone+1);
				printThis(nRows, nCols, new_p);
			}
			if(pause=='y')//if user wants a pause, pause and wait for input
			{
				puts("Paused waiting for input.");
				getc(stdin);//just waits for user input
			}

			//musical pointers
			spare_p = other_p;
			other_p = old_p;
			old_p = new_p;
			new_p = spare_p;

		}

	}

	//If repeated pattern and organisms dead are false, it must have ended due to user-specified max gens
	//Let user know terminating condition and print array
	if (!patternRepeated && !allOrganismsDead){
		puts("Pattern terminated after user-specified generations.");
		puts("Final pattern is:");
		printThis(nRows, nCols, old_p);
	}

	return g;//return total number of generations
}

/**
 * determines if entire array is empty
 * @param arr: pointer to character array under determination
 * @param nRows: number of rows in array
 * @param nCols: number of columns in array
 * @return true if any cells are occupied, false if entire array is empty
 */
bool anyX(char* arr, int nRows, int nCols)
{
	bool any = false;//whether array has any x's, initialized to false
	for(int row=0; !any && (row<nRows); row++)//loops through every row and column, checking if the letter x is found
	{
		for(int col=0; !any && (col< nCols); col++)
		{
			if(getLetter(row, col, nCols, arr) == 'x')//if x is found, set any to true
			{
				any=true;
			}
		}
	}
	return any;//return whether or not array is empty
}

/**
 * determines if two arrays are the same
 * @param one_p: pointer to the first array for comparison
 * @param another_p: pointer to the second array for comparison
 * @param nRows: number of rows in arrays
 * @param nCols: number of columns in arrays
 * @return whether or not the arrays are the same
 * 					true if arrays are same
 * 					false if arrays are different
 */
bool sameContent(char* one_p, char* another_p, int nRows, int nCols)
{
	bool same = true;//whether the arrays are the same, initialized to true

	for(int row=0; same && (row<nRows); row++)//loops through every row and column in both arrays, comparing each cell
	{
		for(int col=0; same && (col< nCols); col++)
		{
			//if the letter in each is not the same, set same to false
			if(getLetter(row, col, nCols, one_p) != getLetter(row, col, nCols, another_p))
			{
				same=false;
			}
		}
	}
	return same;//return whether arrays are the same

}

/**
 * prints the array
 * @param nRows: number of rows in array
 * @param nCols: number of columns in array
 * @param old_p: array to print
 * @return void, prints the array
 */
void printThis(int nRows, int nCols, char* old_p)
{
	for(int row=0;row<nRows;row++)//loops through each row and column, printing the character in the cell
	{
		for(int col=0;col<nCols;col++)
		{
			if (*(old_p+(row*nCols)+col) == 'o'){
				printf(" ");
			}
			else{
				printf("%c", *(old_p+(row*nCols)+col));
			}
		}
		printf("\n");//when the row is finished, prints a newline
	}
}

