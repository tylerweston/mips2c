/*
	mips2c memory
		- holds memory and main implementation of it
*/

#include "headers/mips2c.h"
#include "headers/memory.h"
#include "headers/registers.h"

char* memory[MEMORY_SIZE];

void write_memory(char *item, char* mem_loc, int size)
{
	memcpy(mem_loc, item, size);
}

void get_memory(char *save_loc, char* mem_loc, int size)
{
	memcpy(save_loc, mem_loc, size);
}

void clear_memory()

{
	memset(memory, 0, MEMORY_SIZE);
	registers[_$SP] = MEMORY_SIZE / 2;	// clear stack pointer
}

void print_memory()
{
	PRINT("Displaying Memory:\n");
	PRINT("------------------\n");
	for (int x = 0; x < MEMORY_SIZE; x++)
	{
		// TODO: Let's display this in hex?
		if (x % 16 == 0) 
			PRINT(ANSI_COLOR_BRIGHT_GREEN "\n%d:" ANSI_COLOR_RESET, x);
		if (x % 4 == 0) 
			PRINT("\t");
		PRINT("%c", memory[x]);
	}
	PRINT("\n");
}