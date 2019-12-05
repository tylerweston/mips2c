/*
	mips2c memory
		- holds memory and main implementation of it
*/

#include "mips2c.h"

// TODO:

void init_memory()
{
	memory = malloc(MEMORY_SIZE);
	clear_memory();
}

void write_mem(void *item, void* mem_loc, int size)
{
	memcpy(mem_loc, item, size);
}

void get_mem(void *save_loc, void* mem_loc, int size)
{
	memcpy(save_loc, mem_loc, size);
}

void clear_memory()
{
	memset(memory, 0, MEMORY_SIZE);
}

void print_memory()
{
	printf("displaying memory:\n");
	printf("%s\n",(char*)memory);
	// for (int x = 0; x<MEMORY_SIZE; x++)
	// {
	// 	printf("%c",(char) memory[x]);
	// 	if (x%4 == 0) printf(" ");
	// 	if (x%32 == 0) printf("\n");
	// }
}