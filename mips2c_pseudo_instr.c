/*
	mips2c pseudoinstruction handler
*/

// todo:
// better to implement these directly or translate them to their corresponding pure mips before?
// whatever, either way is probably fine??

#include "mips2c.h"

void _move(int32_t *t, int32_t* s)
{
	*t = *s;
}

void _clear(int32_t *t)
{
	*t = 0;
}

void _li(int32_t *t, int imm)
{
	*t = imm;
}

void _la(int32_t *t, char* label)
{
	// todo, this!
	// search our linked list of labels and return
	// the pointer to where that data lives in 
	// memory
}

// todo: what is the type of address? is it a char* label?
// void _la(int32_t *t, int address)
// {
// 	*t = address
// }

// https://github.com/MIPT-ILab/mipt-mips/wiki/MIPS-pseudo-instructions

// TODO:
// branch pseudoinstructions

// todo: how to implement this?
// todo: how to avoid collision with other mult?
// void _mul(int32_t *d, int32_t *s, int32_t *t)
// {
// 	*d = (*s * *t) & 0xFFFFFFFF
// }

// todo: how to avoid collisions with other div function?
// void _div(int32_t *d, int32_t *s, int32_t *t)
// {
// 	*d = *s / *t;
// }

void _rem(int32_t *d, int32_t *s, int32_t *t)
{
	*d = *s % *t;
}

void _not(int32_t *t, int32_t *s)
{
	*t = ~*s;
}

void _nop()
{
	// no op
}

void _b(char* label)
{
	// branch unconditional
	_beq(0, 0, label);
}