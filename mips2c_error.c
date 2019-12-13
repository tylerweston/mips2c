/*
	mips2c_errors
*/
#include "mips2c.h"


// plump this up? print to stderr as well for error logging?
void error(char* error)
{
	printf(ANSI_COLOR_RED "\nERROR" ANSI_COLOR_RESET ": %s\n", error);
	fprintf(stderr, "%s\n", error);
	exit(1);
}