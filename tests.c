/*
 * tests.c
 *
 *  Created on: Jan 30, 2019
 *      Author: Hava Kantrowitz
 */
#include <stdbool.h>
#include "tests.h"
#include "production.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for strncmp

/**
 * determines if all tests run properly
 * @return true if all tests run successfully, false otherwise
 */
bool tests(void)
{
	bool results=false;
	puts("During tests");
	bool ok1 = testReadInput();
	if(ok1)puts("Found and read the test file.");
	bool ok2 = testMakeArrays();
	if(ok2)puts("Was able to allocate the arrays ok.");
	bool ok3 = testPlayOneSteadyState();
	if(ok3)puts("One generation, steady state, is ok.");
	bool ok4 = testPlayOneBlinker();
	if(ok4)puts("Blinker one generation is ok.");
	bool ok5 = testPlayOneToad();
	if(ok5)puts("Toad one generation is ok.");
	puts("TESTS RAN SUCCESSFULLY. END OF TESTS.");
	results = ok1 && ok2 && ok3 && ok4 && ok5;
	printf("tests returning %d.\n",results);
	return results;
}

/**
 * determines if the ability to read the input file works properly
 * @return true if the test runs correctly, false otherwise
 */
bool testReadInput(void)
{
	//the plan is:
	//create a file
	//make sure it can be opened.
	//try a file that should not exist
	//check that we notice it does not exist

	bool results = false;//overall results of test, initialized to false
	bool ok1 = false;//result of test element 1, initialized to false
	bool ok2 = false;//result of test element 2, initialized to false
	bool ok3 = false;//result of test element 3, initialized to false
	//
	//Here goes: create a file
	FILE* fp;//create a pointer to a file
	fp = fopen("/tmp/LifeInput.txt", "w"); //we write it first for later reading
	if (fp != false)
	{//it opened, yay!
		//file containing a sequence of lines,
		//each consisting of a sequence of 'x' and 'o' characters,
		//indicating the occupied and unoccupied cells
		//of the initial configuration, e.g.
		//oxo
		//xox
		//xox
		//oxo
		int w1 = fprintf(fp, "oxo\n");//returns the number of chars excluding null
		int w2 = fprintf(fp, "xox\n");
		int w3 = fprintf(fp, "xox\n");
		int w4 = fprintf(fp, "oxo\n");
		if ((w1 == w2) && (w2 == w3) && (w3 == w4) && (w4 == 4))//if each line has 4 characters
		{
			ok1 = true;
		}
		fclose(fp);
	}
	fp = fopen("/tmp/LifeInput.txt", "r"); //we try to read it
	if (fp != false)
	{//it opened, yay!
		ok2 = true;
		//can we read the correct data?
		char oRow[4]; //test case, we know a string of 3 chars terminated by null
		int count =  fscanf(fp, "%s", oRow);//scan for row
		if(0==strncmp(oRow, "oxo", 4))//0 means match
		{//got the right data
			ok3 = true;
		}
		fscanf(fp, "%s", oRow);//scan for row
		if(0==strncmp(oRow, "xox", 4))//0 means match
		{//got the right data
			ok3 = true;
		}
		fscanf(fp, "%s", oRow);//scan for row
		if(0==strncmp(oRow, "xox", 4))//0 means match
		{//got the right data
			ok3 = true;
		}
		fscanf(fp, "%s", oRow);//scan for row
		if(0==strncmp(oRow, "oxo", 4))//0 means match
		{//got the right data
			ok3 = true;
		}
	}
	results = ok1 && ok2 && ok3;//set overall results to the result of the 3 aspects of the larger test
	return results;//returns whether or not all tests passed
}

/**
 * determines if the creation of arrays works properly
 * @return true if the test runs correctly, false otherwise
 */
bool testMakeArrays(void)
{
	bool results = false;//whether or not test passed, initialized to false
	bool ok1 = false;//whether or not the first segment of the test passed, initialized to false
	//we need to allocate and populate arrays
	//In this test case we set our arrays to 4 rows x3 columns
	int nRows = 4;
	int nCols = 3;
	char boardBefore[4][3]={
			{'o','x','o'},
			{'x','o','x'},
			{'x','o','x'},
			{'o','x','o'}
	};


	char boardAfter[nRows][nCols];//the board that will be created after the new array is made
	//here's the test
	for(int row=0;row<nRows;row++)//loops through every row and column, setting each cell in boardAfter to corresponding cell in boardBefore
	{
		for(int col=0; col<nCols; col++)
		{
			boardAfter[row][col]=boardBefore[row][col];
		}
	}
	ok1 = true;//The allocation happened successfully, set test element passing to false
	//Loops through every row and column, checking if each cell in the two arrays is the same
	//If they aren't the same, set the test element passing to false
	for(int row=0;row<nRows;row++)
	{
		for(int col=0; col<nCols; col++)
		{
			if (boardAfter[row][col]!=boardBefore[row][col])
			{
				ok1=false;
			}
		}
	}

	results = ok1;//set the overall results equal to the result of the first test element
	return results;//return overall results
}

/**
 * determines if the run of one stable generation works correctly
 * @return true if the test runs correctly, false otherwise
 */
bool testPlayOneSteadyState(void)
{
	bool results = false;//overall result of the test, initialized to false
	bool ok1 = false;//result of test element 1, initialized to false

	int nRows = 4;//set number of rows in test board to 4
	int nCols = 3;//set number of columns in test board to 3
	char boardBefore[4][3]={
			{'o','o','o'},
			{'x','x','o'},
			{'x','x','o'},
			{'o','o','o'}
	};//initialize the board before a generation is played

	char boardAfter[nRows][nCols];//create the board that the new generation will write into

	//here's the invocation
	PlayOne(nRows, nCols, (char*)boardBefore, (char*)boardAfter);
	//here's the check
	ok1 = true; //no errors found yet
	//loop through every row and column, checking to see if the boards before and after are the same
	for(int row=0;row<nRows;row++)
	{
		for(int col=0; col<nCols; col++)
		{
			if(boardAfter[row][col]!=boardBefore[row][col])
				//if a discrepancy is found, set test element 1 to false and let user know where issue occurred
			{
				ok1 = false;
				printf("error found: %c does not equal %c. Row is %d, column is %d.\n", boardBefore[row][col], boardAfter[row][col], row, col);
			}

		}
	}
	results = ok1;//set the overall results equal to the result of the first test element
	return results;//return overall results

}

/**
 * tests that an unstable configuration of a blinker runs correctly
 * @return true if it runs correctly, false otherwise
 */
bool testPlayOneBlinker(void){
	bool results = false;//overall result of the test, initialized to false
	bool ok1 = false;//result of test element 1, initialized to false

	int nRows = 4;//set number of rows in test board to 4
	int nCols = 3;//set number of columns in test board to 3
	char boardBefore[4][3]={
			{'o','o','o'},
			{'o','x','o'},
			{'o','x','o'},
			{'o','x','o'}
	};//initialize the board before a generation is played

	char correctBoardAfter[4][3]={
			{'o','o','o'},
			{'o','o','o'},
			{'x','x','x'},
			{'o','o','o'}
	};//create board that should be the result of generation, to test created board against

	char boardAfter[nRows][nCols];//create the board that the new generation will write into

	//here's the invocation
	PlayOne(nRows, nCols, (char*)boardBefore, (char*)boardAfter);
	//here's the check
	ok1 = true; //no errors found yet
	//loop through every row and column, checking to see if the created board and correct board
	for(int row=0;row<nRows;row++)
	{
		for(int col=0; col<nCols; col++)
		{
			if(boardAfter[row][col]!=correctBoardAfter[row][col])
				//if a discrepancy is found, set test element 1 to false and let user know where issue occurred
			{
				ok1 = false;
				printf("error found: %c does not equal %c. Row is %d, column is %d.\n", boardBefore[row][col], boardAfter[row][col], row, col);
			}

		}
	}
	results = ok1;//set the overall results equal to the result of the first test element
	return results;//return overall results
}

/**
 * tests that an unstable configuration of a toad runs correctly
 * @return true if it runs correctly, false otherwise
 */
bool testPlayOneToad(void){
	bool results = false;//overall result of the test, initialized to false
	bool ok1 = false;//result of test element 1, initialized to false

	int nRows = 4;//set number of rows in test board to 4
	int nCols = 4;//set number of columns in test board to 4
	char boardBefore[4][4]={
			{'o','o','o','o'},
			{'o','o','o','o'},
			{'o','o','o','o'},
			{'o','o','o','o'}
	};//initialize the board before a generation is played

	char correctBoardAfter[4][3]={
			{'o','o','o','o'},
			{'o','o','o','o'},
			{'o','o','o','o'},
			{'o','o','o','o'}
	};//create board that should be the result of generation, to test created board against

	char boardAfter[nRows][nCols];//create the board that the new generation will write into

	//here's the invocation
	PlayOne(nRows, nCols, (char*)boardBefore, (char*)boardAfter);
	//here's the check
	ok1 = true; //no errors found yet
	//loop through every row and column, checking to see if the created board and correct board
	for(int row=0;row<nRows;row++)
	{
		for(int col=0; col<nCols; col++)
		{
			if(boardAfter[row][col]!=correctBoardAfter[row][col])
				//if a discrepancy is found, set test element 1 to false and let user know where issue occurred
			{
				ok1 = false;
				printf("error found: %c does not equal %c. Row is %d, column is %d.\n", boardBefore[row][col], boardAfter[row][col], row, col);
			}

		}
	}
	results = ok1;//set the overall results equal to the result of the first test element
	return results;//return overall results
}

bool testEndThreeGens(void){}
bool testEndOnePreviousGen(void){}
bool testEndOneGrandfatherGen(void){}

bool testGetLetterX(void){}
bool testGetLetterO(void){}

bool testNoNeighbors(void){}
bool testFourNeighbors(void){}
bool testEightNeighbors(void){}

bool testAnyXArrayOccupied(void){}
bool testAnyXArrayEmpty(void){}

bool testSameContentSameArrays(void){}
bool testSameContentDifferentArrays(void){}
