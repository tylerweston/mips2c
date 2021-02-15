/*
	mips2c
	j type instruction handler	
*/

#include "headers/mips2c.h"
#include "headers/instructions.h"
#include "headers/registers.h"

void _j(char* label)
{
	pc = get_line_from_labels(label);
}

void _jal(char* label)
{
	registers[_$RA] = pc + 1;
	pc = get_line_from_labels(label);
}
