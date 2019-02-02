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
	bool ok3 = testPlayOne();
	if(ok3)puts("PlayOne is ok.");
	bool ok4 = testGenerate();
	if(ok4)puts("Generate is ok.");
	bool ok5 = testGetLetter();
	if(ok5)puts("Getting letter is ok.");
	bool ok6 = testNeighbors();
	if(ok6)puts("Finding neighbors is ok.");
	bool ok7 = testAnyX();
	if(ok7)puts("AnyX is ok.");
	bool ok8 = testSameContent();
	if(ok8)puts("Array comparison is ok.");
	puts("END OF TESTS.");
	results = ok1 && ok2 && ok3 && ok4 && ok5 && ok6 && ok7 && ok8;
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
 * determines if play one helper runs correctly
 * @return true if the test runs correctly, false otherwise
 */
bool testPlayOne(void)
{
	bool results = false;//overall result of the test, initialized to false
	bool ok1 = false;//result of steady state test, initialized to false
	bool ok2 = false;//result of blinker test, initialized to false
	bool ok3 = false;//result of toad test, initialized to false

	//steady state test
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

	//blinker test
	char boardBefore2[4][3]={
			{'o','o','o'},
			{'o','x','o'},
			{'o','x','o'},
			{'o','x','o'}
	};//initialize the board before a generation is played

	char correctBoardAfter2[4][3]={
			{'o','o','o'},
			{'o','o','o'},
			{'x','x','x'},
			{'o','o','o'}
	};//create board that should be the result of generation, to test created board against

	char boardAfter2[nRows][nCols];//create the board that the new generation will write into

	//here's the invocation
	PlayOne(nRows, nCols, (char*)boardBefore2, (char*)boardAfter2);
	//here's the check
	ok2 = true; //no errors found yet
	//loop through every row and column, checking to see if the created board and correct board
	for(int row=0;row<nRows;row++)
	{
		for(int col=0; col<nCols; col++)
		{
			if(boardAfter2[row][col]!=correctBoardAfter2[row][col])
				//if a discrepancy is found, set test false and let user know where issue occurred
			{
				ok2 = false;
				printf("error found: %c does not equal %c. Row is %d, column is %d.\n", boardBefore[row][col], boardAfter[row][col], row, col);
			}

		}
	}

	//toad test
	int nRows3 = 4;//set number of rows in test board to 4
	int nCols3 = 4;//set number of columns in test board to 4
	char boardBefore3[4][4]={
			{'o','o','o','o'},
			{'o','x','x','x'},
			{'x','x','x','o'},
			{'o','o','o','o'}
	};//initialize the board before a generation is played

	char correctBoardAfter3[4][4]={
			{'o','o','x','o'},
			{'x','o','o','x'},
			{'x','o','o','x'},
			{'o','x','o','o'}
	};//create board that should be the result of generation, to test created board against

	char boardAfter3[nRows3][nCols3];//create the board that the new generation will write into

	//here's the invocation
	PlayOne(nRows3, nCols3, (char*)boardBefore3, (char*)boardAfter3);
	//here's the check
	ok3 = true; //no errors found yet
	//loop through every row and column, checking to see if the created board and correct board
	for(int row=0;row<nRows3;row++)
	{
		for(int col=0; col<nCols3; col++)
		{
			if(boardAfter3[row][col]!=correctBoardAfter3[row][col])
				//if a discrepancy is found, set test element 1 to false and let user know where issue occurred
			{
				ok3 = false;
				printf("error found: %c does not equal %c. Row is %d, column is %d.\n", boardBefore3[row][col], boardAfter[row][col], row, col);
			}

		}
	}
	results = ok1 && ok2 && ok3;//set the overall results equal to the result of all individual tests
	return results;//return overall results

}

/**
 * tests generate function
 * @return true if function works correctly, false otherwise
 */
bool testGenerate(void){
	bool results = false;//result of overall test, initialized to false
	bool ok1 = false;//result of dead board test, initialized to false
	bool ok2 = false;//result of user specified end test, initialized to false
	bool ok3 = false;//result of previous generation test, initialized to false
	bool ok4 = false;//result of grandfather generation test, initialized to false

	//dead board test

	int gens = 3;//set number of generations to 3
	int nRows = 2;//set number of rows to 2
	int nCols = 2;//set number of columns to 2

	char emptyBoard1[2][2] = {
		{'o','o'},
		{'o','o'}
	};//empty board to initialize array a

	char emptyBoard2[2][2] = {
		{'o','o'},
		{'o','o'}
	};//empty board to initialize array b

	char firstGenBoard[6][6] = {
		{'o','o'},
		{'x','o'}
	};//first iteration of the board to initialize first generation of array

	char print = 'n';//initialize print to no
	char pause = 'n';//initialize pause to no

	int numGens = generate(gens, nRows, nCols, firstGenBoard, emptyBoard1, emptyBoard2, print, pause);
	//expected end after 1 generation
	if (numGens == 1){//if the number of generations is the expected number, set results to true
		ok1 = true;
	}
	else{//otherwise let user know error occured, and how many generations actually ran
		printf("%d generations occurred\n", numGens);
	}

	//user specified test

	int gens2 = 3;//set number of generations to 3
	int nRows2 = 6;//set number of rows to 6
	int nCols2 = 6;//set number of columns to 6

	char emptyBoard1B[6][6] = {
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
	};//empty board to initialize array a

	char emptyBoard2B[6][6] = {
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
	};//empty board to initialize array b

	char firstGenBoardB[6][6] = {
		{'o','o','x','o','o','o'},
		{'o','o','o','x','o','o'},
		{'o','x','x','x','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
	};//first iteration of the board to initialize first generation of array

	int numGens2 = generate(gens2, nRows2, nCols2, firstGenBoardB, emptyBoard1B, emptyBoard2B, print, pause);
	//expected end after 3 generations
	if (numGens2 == 3){//if the number of generations is the expected number, set results to true
		ok2 = true;
	}
	else{//otherwise let user know error occured, and how many generations actually ran
		printf("%d generations occurred\n", numGens2);
	}
	//previous gen test

	int gens3 = 10;//number of max generations
	int nRows3 = 2;//number of rows in array
	int nCols3 = 2;//number of columns in array

	char emptyBoard1C[][2] = {
		{'o','o'},
		{'o','o'}
	};//empty board to initialize array a

	char emptyBoard2C[][2] = {
		{'o','o'},
		{'o','o'}
	};//empty board to initialize array b

	char firstGenBoardC[2][2] = {
		{'x','x'},
		{'x','x'}
	};//first iteration of the board to initialize first generation of array

	int numGens3 = generate(gens3, nRows3, nCols3, firstGenBoardC, emptyBoard1C, emptyBoard2C, print, pause);
	//expected end after 1 generation
	if (numGens3 == 1){//if the number of generations is the expected number, set results to true
		ok3 = true;
	}
	else{//otherwise let user know error occured, and how many generations actually ran
		printf("%d generations occurred\n", numGens3);
	}

	//grandfather gen test

	int gens4 = 10;//number of max generations
	int nRows4 = 6;//number of rows in array
	int nCols4 = 6;//number of columns in array

	char emptyBoard1D[6][6] = {
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
	};//empty board to initialize array a

	char emptyBoard2D[6][6] = {
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
	};//empty board to initialize array b

	char firstGenBoardD[6][6] = {
		{'o','o','o','o','o','o'},
		{'o','o','x','o','o','o'},
		{'o','o','x','o','o','o'},
		{'o','o','x','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
	};//initialization of first iteration of board

	int numGens4 = generate(gens4, nRows4, nCols4, firstGenBoardD, emptyBoard1D, emptyBoard2D, print, pause);
	//expected end after 2 generations
	if (numGens4 == 2){//if the number of generations is the expected number, set results to true
		ok4 = true;
	}
	else{//otherwise let user know error occured, and how many generations actually ran
		printf("%d generations occurred\n", numGens4);
	}

	results = ok1 && ok2 && ok3 && ok4;//set overall result to results of individual test elements
	return results;//return the result of the tests
}


/**
 * tests if functions grabs letter correctly
 * @return true if test works, false if test fails
 */
bool testGetLetter(void){
	bool results = false;//result of test, initialized to false

	int row = 1;//row of cell
	int column = 0;//column of cell
	int nCols = 2;//number of columns in array
	char testArray[2][2] = {
			{'o','o'},
			{'x','o'}
	};//array to test with

	char letter = getLetter(row, column, nCols, testArray);
	if (letter == 'x'){//if the letter is x, set results to true
		results = true;
	}

	return results;//return the result of test
}

/**
 * tests if function counts number of neighbors correctly
 * @return true if test runs successfully, false otherwise
 */
bool testNeighbors(void){
	bool results = false;//result of overall test, initialized to 0
	bool ok1 = false;//test with no neighbors, initialized to 0
	bool ok2 = false;//test with all neighbors, initialized to 0

	//no neighbors

	int row = 0;//row of cell to test
	int column = 0;//row of column to test
	int nRows = 2;//number of rows in array
	int nCols = 2;//number of columns in array
	char testArray[2][2]={
		{'o','o'},
		{'o','o'}
	};//array to test

	int numNeighbors = HowManyNeighbors(row, column, nRows, nCols, testArray);
	if (numNeighbors == 0){//if number of neighbors is 0, set results to true
		ok1 = true;
	}

	//eight neighbors

	int row2 = 1;//row of cell to test
	int column2 = 1;//column of cell to test
	int nRows2 = 3;//number of rows in array
	int nCols2 = 3;//number of columns in array
	char testArrayB[3][3]={
		{'x','x','x'},
		{'x','o','x'},
		{'x','x','x'}
	};//array to test

	int numNeighbors2 = HowManyNeighbors(row2, column2, nRows2, nCols2, testArrayB);
	if (numNeighbors2 == 8){//if array has 8 neighbors, set results to true
		ok2 = true;
	}

	results = ok1 && ok2;//set result of overall test to result of individual elements
	return results;//return result of tests
}


/**
 * tests if anyX function correctly determines if any cells are occupied
 * @return true if test runs successfully, false otherwise
 */
bool testAnyX(void){
	bool results = false;//result of overall test, initialized to false
	bool ok1 = false;//array has an x in it, initialized to false
	bool ok2 = false;//array does not have an x in it, initialized to false;

	//test with x

	int nRows = 2;//number of rows in array
	int nCols = 2;//number of columns in array
	char testArray[2][2]={
		{'x','o'},
		{'o','o'}
	};//initializes test array

	bool hasX = anyX(testArray, nRows, nCols);
	//If any cell has an x in it, set results to true
	if (hasX){
		ok1 = true;
	}

	//test without x
	char testArrayB[2][2]={
		{'o','o'},
		{'o','o'}
	};//initializes test array

	bool hasX2 = anyX(testArrayB, nRows, nCols);
	//If no cell is occupied, set results to true
	if (!hasX2){
		ok2 = true;
	}

	results = ok1 && ok2;//sets result of overall test to results of individual tests
	return results;//return result of test
}

/**
 * tests if sameContent function correctly determines if two arrays are the same
 * @return true if test runs successfully, false otherwise
 */
bool testSameContent(void){
	bool results = false;//result of overall test, initialized to false
	bool ok1 = false;//test with arrays the same, initialized to false
	bool ok2 = false;//test with arrays different, initialized to false

	//same arrays

	int nRows = 2;//number of rows in array
	int nCols = 2;//number of columns in array
	char testArray1[2][2]={
		{'o','o'},
		{'x','x'}
	};//initialization of first comparison array

	char testArray2[2][2]={
		{'o','o'},
		{'x','x'}
	};//initialization of second comparison array

	bool isSame = sameContent(testArray1, testArray2, nRows, nCols);
	//if the arrays are the same, set result to true
	if(isSame){
		ok1 = true;
	}

	//different arrays
	char testArray1B[2][2]={
		{'o','o'},
		{'x','x'}
	};//initialization of first comparison array

	char testArray2B[2][2]={
		{'x','x'},
		{'o','o'}
	};//initialization of second comparison array

	bool isSame2 = sameContent(testArray1B, testArray2B, nRows, nCols);
	//if the arrays are different, set the result to true
	if(!isSame2){
		ok2 = true;
	}

	results = ok1 && ok2;//set overall result to results of individual tests
	return results;//return result of test
}
