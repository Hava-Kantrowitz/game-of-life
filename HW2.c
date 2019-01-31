/*
 ============================================================================
 Name        : HW2.c
 Author      : Hava Kantrowitz
 Version     : 1/30/19
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "tests.h"
#include "production.h"

/**
 * Main function to start program and ensure production and tests
 * go smoothly
 * @param argc Number of words on the command line
 * @param argv Array of pointers to character strings representing the words on the command line.
 * @return exit_success if all code in main ran successfully
 */
int main(int argc, char* argv[])
{
	bool ok=false;//variable to determine if game of life works, initialized to not working

	puts("!!!Hi all. We're doing HW2!!!");
	if(tests())//if all tests work, play game
	{
		ok=production(argc, argv);
		if (ok)//if game of life works, let user know
		{
			puts("Game of life seems to have worked.");
		}
		else//ifnot, let user know as well
		{
			puts("Game of life seems not to have worked.");
		}
	}
	else//let user know tests did not work
	{
		puts("Something went wrong in the tests.");
	}
	return EXIT_SUCCESS;
}
