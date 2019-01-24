/*
 * production.c
 *
 *  Created on: Nov 3, 2018
 *      Author: student
 */

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "production.h"
#include <math.h>

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
