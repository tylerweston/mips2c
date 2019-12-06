/*
	mips2c memory
		- holds memory and main implementation of it
*/

#include "mips2c.h"

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
	memset(memory, 0, MEMORY_SIZE);
}

void print_memory()
{
	// todo: make this look better
	printf("Displaying Memory:\n");
	printf("------------------\n");
	for (int x = 0; x < MEMORY_SIZE; x++)
	{
		printf("%u", memory[x]);
		if (x % 4 == 0) printf("\t");
		if (x % 16 == 0) printf("\n%d:", x);
	}
	printf("\n");
}