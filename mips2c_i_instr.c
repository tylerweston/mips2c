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
