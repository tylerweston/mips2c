/*
	mips2c
	j type instruction handler	
*/

#include "mips2c.h"

void _j(char* label)
{
	if (debug) printf("_j %s\n", label);
	pc = get_line_from_labels(label);
}

void _jal(char* label)
{
	if (debug) printf("_jal %s\n", label);
	registers[_$RA] = pc + 1;
	pc = get_line_from_labels(label);
}
