/*
	mips2c memory
		- holds memory and main implementation of it
*/

#include "mips2c.h"

// 

void write_memory(unsigned char *item, unsigned char* mem_loc, int size)
{
	memcpy(mem_loc, item, size);
	// memcpy(disk + address, &x, sizeof(unsigned int));
}

void get_memory(unsigned char *save_loc, unsigned char* mem_loc, int size)
{
	memcpy(save_loc, mem_loc, size);
}

void clear_memory()

{
	memset(data_segment, 0, MEMORY_SIZE);
	memset(heap, 0, MEMORY_SIZE);
	memset(stack, 0, MEMORY_SIZE);
	registers[_$SP] = 0;	// clear stack pointer
}

void print_memory()
{
	// todo: make this look better
	printf("Displaying Memory:\n");
	printf("------------------\n");
	printf(ANSI_COLOR_BLUE "Data segment\n" ANSI_COLOR_RESET);
	for (int x = 0; x < MEMORY_SIZE; x++)
	{
		if (x % 16 == 0) printf("\n%d:", x);
		if (x % 4 == 0) printf("\t");

		printf("%c", data_segment[x]);
	}
	printf(ANSI_COLOR_BLUE "\n\nHeap\n" ANSI_COLOR_RESET);
	for (int x = 0; x < MEMORY_SIZE; x++)
	{
		if (x % 16 == 0) printf("\n%d:", x);
		if (x % 4 == 0) printf("\t");

		printf("%c", heap[x]);

	}
	printf(ANSI_COLOR_BLUE "\n\nStack\n" ANSI_COLOR_RESET);
	for (int x = 0; x < MEMORY_SIZE; x++)
	{
		if (x % 16 == 0) printf("\n%d:", x);
		if (x % 4 == 0) printf("\t");

		printf("%c", stack[x]);
	}	
	printf("\n");
}