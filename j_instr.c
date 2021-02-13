/*
	mips2c
	j type instruction handler	
*/

#include "mips2c.h"
#include "instructions.h"
#include "registers.h"

void _j(char* label)
{
	pc = get_line_from_labels(label);
}

void _jal(char* label)
{
	registers[_$RA] = pc + 1;
	pc = get_line_from_labels(label);
}
