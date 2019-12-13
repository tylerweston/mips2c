/*
	mips2c
	I-type instructions
*/

#include "mips2c.h"
// helper
int get_line_from_labels(char* search_label);

void _beq(int32_t *s, int32_t* t, char* label)
{
	if (*s == *t)
	{
		// set pc to line pointed to by label
		pc = get_line_from_labels(label);
	}
}

void _bne(int32_t *s, int32_t* t, char* label)
{
	if (*s != *t)
	{
		// set pc to line pointed to by label
		pc = get_line_from_labels(label);
	}
}

void _blez(int32_t *s, char* label)
{
	if (*s <= 0)
	{
		// set pc to line pointed to by label
		pc = get_line_from_labels(label);
	}
}

void _bgtz(int32_t *s, char* label)
{
	if (*s > 0)
	{
		// set pc to line pointed to by label
		pc = get_line_from_labels(label);
	}
}

void _addi(int32_t *t, int32_t *s, int32_t imm)
{
	// todo: signed!
	*t = *s + imm;
}

void _addiu(int32_t *t, int32_t *s, int32_t imm)
{
	// todo: unsigned!
	*t = *s + imm;
}

void _slti(int32_t *t, int32_t *s, int32_t imm)
{
	*t = *s < imm ? 1 : 0;
}

void _sltiu(int32_t *t, int32_t *s, int32_t imm)
{
	*t = *s < imm ? 1 : 0;
}

void _andi(int32_t *t, int32_t *s, int32_t imm)
{	
	// todo: test this, is it correct?
	*t = *s & imm;
}

void _ori(int32_t *t, int32_t *s, int32_t imm)
{
	*t = *s | imm;
}

void _xori(int32_t *t, int32_t *s, int32_t imm)
{
	// todo: test this?!
	*t = *s ^ imm;
}

// Memory

void _lb(int32_t *t, int32_t* s, int32_t offset)
{
	// t = *(int8*)(s + offset);
}

void _lh(int32_t *t, int32_t* s, int32_t offset)
{
	// t = *(int8*)(s + offset);
}

void _lbu(int32_t *t, int32_t* s, int32_t offset)
{
	// t = *(int8*)(s + offset);
}

void _lhu(int32_t *t, int32_t* s, int32_t offset)
{
	// t = *(int8*)(s + offset);
}

void _lw(int32_t *t, int32_t* s, int32_t offset)
{
	*t = *s + offset;
}

void _lui(int32_t *t, int32_t offset)
{
	// to do
}

void _sb(int32_t *t, int32_t* s, int32_t offset)
{
	// t = *(int8*)(s + offset);
}

void _sh(int32_t *t, int32_t* s, int32_t offset)
{
	// t = *(int8*)(s + offset);
}

void _sw(int32_t *t, int32_t* s, int32_t offset)
{
	// s will be a POINTER into memory somewhere!
	// (our HEAP)
	*t = *s + offset;
}


// helper functions here!

int get_line_from_labels(char* search_label)
{
	// print_labels();
	label_list *curr = labels;
	while (curr != NULL)
	{
		if (strcmp(curr->label, search_label) == 0)
		{
			return curr->source_line;
		}
		// printf("label: %s line number: %d\n", curr->label, curr->source_line);
		curr = curr->next;
	}
	printf(ANSI_COLOR_RED "ERROR" ANSI_COLOR_RESET ": Cannot find label %s\n", search_label);
	exit(1);
}
