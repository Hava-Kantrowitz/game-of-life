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
	//if(ok1)puts("Found and read the test file.");
	bool ok2 = testMakeArrays();
	//if(ok2)puts("Was able to allocate the arrays ok.");
	bool ok3 = testPlayOneSteadyState();
	//if(ok3)puts("One generation, steady state, is ok.");
	bool ok4 = testPlayOneBlinker();
	//if(ok4)puts("Blinker one generation is ok.");
	bool ok5 = testPlayOneToad();
	//if(ok5)puts("Toad one generation is ok.");
	bool ok6 = testEndThreeGens();
	//if(ok6)puts("Termination after 3 generations is ok.");
	bool ok7 = testEndOnePreviousGen();
	//if(ok7)puts("Termination steady state is ok.");
	bool ok8 = testEndOneGrandfatherGen();
	//if(ok8)puts("Termination oscillating state is ok.");
	bool ok9 = testGetLetterX();
	//if(ok9)puts("Grab of x is ok.");
	bool ok10 = testGetLetterO();
	//if(ok10)puts("Grab of o is ok.");
	bool ok11 = testNoNeighbors();
	//if(ok11)puts("No neighbors is ok.");
	bool ok12 = testFourNeighbors();
	//if(ok12)puts("Four neighbors is ok.");
	bool ok13 = testEightNeighbors();
	//if(ok13)puts("All neighbors is ok.");
	bool ok14 = testEndDeadBoard();
	//if(ok14)puts("Termination of dead board is ok.");
	bool ok15 = testAnyXArrayOccupied();
	//if(ok15)puts("Occupied array is ok.");
	bool ok16 = testAnyXArrayEmpty();
	//if(ok16)puts("Empty array is ok.");
	bool ok17 = testSameContentSameArrays();
	//if(ok17)puts("Same content array is ok.");
	bool ok18 = testSameContentDifferentArrays();
	//if(ok18)puts("Different content is ok.");
	puts("END OF TESTS. SEE TESTS.C FOR LIST OF PASSING TESTS.");
	results = ok1 && ok2 && ok3 && ok4 && ok5 && ok6 && ok7 && ok8 && ok9 && ok10 && ok11 && ok12 && ok13
			&& ok14 && ok15 && ok16 && ok17 && ok18;
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
			{'o','x','x','x'},
			{'x','x','x','o'},
			{'o','o','o','o'}
	};//initialize the board before a generation is played

	char correctBoardAfter[4][4]={
			{'o','o','x','o'},
			{'x','o','o','x'},
			{'x','o','o','x'},
			{'o','x','o','o'}
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

bool testEndDeadBoard(void){
	bool results = false;//result of test, initialized to false

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
		results = true;
	}
	else{//otherwise let user know error occured, and how many generations actually ran
		printf("%d generations occurred\n", numGens);
	}

	return results;//return the result of the tests
}

/**
 * tests game that ends at the user-specified number of generations
 * @return true if test runs successfully, false otherwise
 */
bool testEndThreeGens(void){
	bool results = false;//result of test, initialized to false

	int gens = 3;//set number of generations to 3
	int nRows = 6;//set number of rows to 6
	int nCols = 6;//set number of columns to 6

	char emptyBoard1[6][6] = {
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
	};//empty board to initialize array a

	char emptyBoard2[6][6] = {
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
	};//empty board to initialize array b

	char firstGenBoard[6][6] = {
		{'o','o','x','o','o','o'},
		{'o','o','o','x','o','o'},
		{'o','x','x','x','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
	};//first iteration of the board to initialize first generation of array

	char print = 'n';//initialize print to no
	char pause = 'n';//initialize pause to no

	int numGens = generate(gens, nRows, nCols, firstGenBoard, emptyBoard1, emptyBoard2, print, pause);
	//expected end after 3 generations
	if (numGens == 3){//if the number of generations is the expected number, set results to true
		results = true;
	}
	else{//otherwise let user know error occured, and how many generations actually ran
		printf("%d generations occurred\n", numGens);
	}

	return results;//return the result of the tests
}

/**
 * tests game that ends due to one repeated generation
 * @return true if test runs successfully, false otherwise
 */
bool testEndOnePreviousGen(void){
	bool results = false;//result of test, initialized to false

	int gens = 10;//number of max generations
	int nRows = 2;//number of rows in array
	int nCols = 2;//number of columns in array

	char emptyBoard1[][2] = {
		{'o','o'},
		{'o','o'}
	};//empty board to initialize array a

	char emptyBoard2[][2] = {
		{'o','o'},
		{'o','o'}
	};//empty board to initialize array b

	char firstGenBoard[2][2] = {
		{'x','x'},
		{'x','x'}
	};//first iteration of the board to initialize first generation of array

	char print = 'n';//initialize print to no
	char pause = 'n';//initialize pause to no

	int numGens = generate(gens, nRows, nCols, firstGenBoard, emptyBoard1, emptyBoard2, print, pause);
	//expected end after 1 generation
	if (numGens == 1){//if the number of generations is the expected number, set results to true
		results = true;
	}
	else{//otherwise let user know error occured, and how many generations actually ran
		printf("%d generations occurred\n", numGens);
	}

	return results;//return the result of the tests
}

/**
 * tests game that ends due to two repeated generations
 * @return true if test runs successfully, false otherwise
 */
bool testEndOneGrandfatherGen(void){
	bool results = false;//result of test, initialized to false

	int gens = 10;//number of max generations
	int nRows = 6;//number of rows in array
	int nCols = 6;//number of columns in array

	char emptyBoard1[6][6] = {
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
	};//empty board to initialize array a

	char emptyBoard2[6][6] = {
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
	};//empty board to initialize array b

	char firstGenBoard[6][6] = {
		{'o','o','o','o','o','o'},
		{'o','o','x','o','o','o'},
		{'o','o','x','o','o','o'},
		{'o','o','x','o','o','o'},
		{'o','o','o','o','o','o'},
		{'o','o','o','o','o','o'},
	};//initialization of first iteration of board

	char print = 'n';//initialize print to no
	char pause = 'n';//initialize pause to no

	int numGens = generate(gens, nRows, nCols, firstGenBoard, emptyBoard1, emptyBoard2, print, pause);
	//expected end after 2 generations
	if (numGens == 2){//if the number of generations is the expected number, set results to true
		results = true;
	}
	else{//otherwise let user know error occured, and how many generations actually ran
		printf("%d generations occurred\n", numGens);
	}

	return results;//return the result of the tests
}

/**
 * tests if functions grabs x correctly
 * @return true if test works, false if test fails
 */
bool testGetLetterX(void){
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
 * tests if function grabs o correctly
 * @return true if test runs successfully, false otherwise
 */
bool testGetLetterO(void){
	bool results = false;//result of test, initialized to false

	int row = 0;//row of cell
	int column = 1;//column of cell
	int nCols = 2;//number of columns in array
	char testArray[2][2] = {
			{'x','o'},
			{'x','x'}
	};//array to test with

	char letter = getLetter(row, column, nCols, testArray);
	if (letter == 'o'){//if the letter is o, set results to true
		results = true;
	}

	return results;//return the result of test
}

/**
 * tests if array without neighbors returns right number of neighbors
 * @return true if test runs successfully, false otherwise
 */
bool testNoNeighbors(void){
	bool results = false;//result of test, initialized to 0

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
		results = true;
	}

	return results;//return result of tests
}

/**
 * tests if array with 4 neighbors returns right number of neighbors
 * @return true if test runs successfully, false otherwise
 */
bool testFourNeighbors(void){
	bool results = false;//result of test, initialized to 0

	int row = 1;//row of cell to test
	int column = 1;//row of column to test
	int nRows = 3;//number of rows in array
	int nCols = 3;//number of columns in array
	char testArray[3][3]={
		{'x','o','x'},
		{'o','o','o'},
		{'x','o','x'}
	};//array to test

	int numNeighbors = HowManyNeighbors(row, column, nRows, nCols, testArray);
	if (numNeighbors == 4){//if the number of neighbors is 4, set results to true
		results = true;
	}

	return results;//return result of test
}

/**
 * tests if array with 8 neighbors returns right number of neighbors
 * @return true if test runs successfully, false otherwise
 */
bool testEightNeighbors(void){
	bool results = false;//result of test, initialized to false

	int row = 1;//row of cell to test
	int column = 1;//column of cell to test
	int nRows = 3;//number of rows in array
	int nCols = 3;//number of columns in array
	char testArray[3][3]={
		{'x','x','x'},
		{'x','o','x'},
		{'x','x','x'}
	};//array to test

	int numNeighbors = HowManyNeighbors(row, column, nRows, nCols, testArray);
	if (numNeighbors == 8){//if array has 8 neighbors, set results to true
		results = true;
	}

	return results;//return result of tests
}

/**
 * tests if array with at least once occupied cell returns anyX correctly
 * @return true if test runs successfully, false otherwise
 */
bool testAnyXArrayOccupied(void){
	bool results = false;//result of test, initialized to false
	int nRows = 2;//number of rows in array
	int nCols = 2;//number of columns in array
	char testArray[2][2]={
		{'x','o'},
		{'o','o'}
	};//initializes test array

	bool hasX = anyX(testArray, nRows, nCols);
	//If any cell has an x in it, set results to true
	if (hasX){
		results = true;
	}

	return results;//return result of test
}

/**
 * tests if array with no occupied cells returns anyX correctly
 * @return true if test runs successfully, false otherwise
 */
bool testAnyXArrayEmpty(void){
	bool results = false;//result of test, initialized to false
	int nRows = 2;//number of rows in array
	int nCols = 2;//number of columns in array
	char testArray[2][2]={
		{'o','o'},
		{'o','o'}
	};//initializes test array

	bool hasX = anyX(testArray, nRows, nCols);
	//If no cell is occupied, set results to true
	if (!hasX){
		results = true;
	}

	return results;//return result of test
}

/**
 * tests if two arrays are the same
 * @return true if test runs successfully, false otherwise
 */
bool testSameContentSameArrays(void){
	bool results = false;//result of test, initialized to false
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
		results = true;
	}

	return results;//return result of test
}

/**
 * tests if two arrays are the same
 * @return true if test runs successfully, false otherwise
 */
bool testSameContentDifferentArrays(void){
	bool results = false;//result of test, initialized to false
	int nRows = 2;//number of rows in array
	int nCols = 2;//number of columns in array
	char testArray1[2][2]={
		{'o','o'},
		{'x','x'}
	};//initialization of first comparison array

	char testArray2[2][2]={
		{'x','x'},
		{'o','o'}
	};//initialization of second comparison array

	bool isSame = sameContent(testArray1, testArray2, nRows, nCols);
	//if the arrays are different, set the result to true
	if(!isSame){
		results = true;
	}

	return results;//return result of test
}
