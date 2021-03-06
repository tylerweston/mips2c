/*
	mips2c
	r type instruction handler
*/

#include "headers/mips2c.h"

void _sll(int32_t *d, int32_t* t, int32_t shamt)
{
	*d = *t << shamt;
}

void _srl(int32_t *d, int32_t* t, int32_t shamt)
{
	*d = *t << shamt;
}

void _sra(int32_t *d, int32_t* t, int32_t shamt)
{
	*d = *t >> shamt;
}

void _sllv(int32_t *d, int32_t* t, int32_t* s)
{
	*d = *t << *s;
}

void _srlv(int32_t *d, int32_t* t, int32_t* s)
{
	*d = *t << *s;
}

void _srav(int32_t *d, int32_t* t, int32_t* s)
{
	*d = *t >> *s;
}

void _syscall()
{
	// todo: HOW WILL THIS WORK?
}

void _mfhi(int32_t *d)
{
	*d = HI;
}

void _mthi(int32_t *s)
{
	HI = *s;
}

void _mflo(int32_t *d)
{
	*d = LO;
}

void _mtlo(int32_t *s)
{
	LO = *s;
}

void _jr(int32_t* s)
{

	pc = *s;	// *s must be a multiple of 4 (THIS ISN'T HOW PC WORKS)
}

void _jalr(int32_t* d, int32_t *s)
{
	// TO DO!
	// t <- s
	// r <- pc + 8 (?? 2 instructions)
	// _$RA <- pc + 8 (2) (so put NOP after!)
	// pc <- t
}

void _mult(int32_t* s, int32_t *t)
{
	// todo: signed multiplication
	// {HI, LO} = *s * *t
}

void _multu(int32_t* s, int32_t *t)
{
	// todo: unsigned multiplication
	// {HI, LO} = *s * *t
}

void _div(int32_t* s, int32_t *t)
{
	// signed
	LO = *s / *t;
	HI = *s % *t;
}

void _divu(int32_t* s, int32_t *t)
{
	// unsigned
	LO = *s / *t;
	HI = *s % *t;
}

void _add(int32_t* d, int32_t* s, int32_t* t)
{
	// todo: this is signed addition
	if (check_flag(f_debug)) printf("adding!\n");
	*d = *s + *t;
}

void _addu(int32_t* d, int32_t* s, int32_t* t)
{
	// todo: unsigned addition?
	*d = *s + *t;
}

void _sub(int32_t* d, int32_t* s, int32_t* t)
{
	// todo: this is signed subtraction
	*d = *s - *t;
}

void _subu(int32_t* d, int32_t* s, int32_t* t)
{
	// todo: unsigned subtraction
	*d = *s - *t;
}

// void _addi(int32_t* t, int32_t* s, int32_t imm)
// {
// 	// todo: signed
// 	*t = *s + imm;
// }

// void _addiu(int32_t* t, int32_t* s, int32_t imm)
// {
// 	// todo: unsigned
// *t = *s + imm;
// }


void _and(int32_t *d, int32_t* s, int32_t* t)
{
	*d = *s & *t;
}

void _or(int32_t *d, int32_t* s, int32_t* t)
{
	*d = *s | *t;
}

void _xor(int32_t *d, int32_t* s, int32_t* t)
{
	*d = *s ^ *t;
}

void _nor(int32_t *d, int32_t* s, int32_t* t)
{
	// nor?
	*d = ~(*s | *t);
}

void _slt(int32_t *d, int32_t* s, int32_t* t)
{
	// signed
	*d = *s < *t;
}

void _sltu(int32_t *d, int32_t* s, int32_t* t)
{
	// unsigned
	*d = *s < *t;
}



