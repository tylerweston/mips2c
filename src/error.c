/*
	mips2c_errors
*/
#include "headers/mips2c.h"
#include <stdio.h>

// plump this up? print to stderr as well for error logging?
void error(char* error)
{
	PRINT(ANSI_COLOR_RED "\nERROR" ANSI_COLOR_RESET ": %s\n", error);
	// fprintf(stderr, "%s\n", error);
	exit_info();
	exit(1);
}