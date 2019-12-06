/*
	mips2c memory
		- holds memory and main implementation of it
*/

#include "mips2c.h"

// unsigned int x;
// unsigned char ch;

// unsigned int got_int;
// unsigned char got_char;

// int address;

// int main()
// {
// 	unsigned char* disk = malloc(1024);
// 	printf("memtest\n");
// 	x = 69;
// 	ch = 'a';
// 	address = 4;
// 	memcpy(disk + address, &x, sizeof(unsigned int));
// 	address = 0;
// 	memcpy(disk + address, &ch, sizeof(unsigned char));
// 	printf("grabbing stuff from memory\n");
// 	address = 4;
// 	memcpy(&got_int, disk + address, sizeof(unsigned int));
// 	address = 0;
// 	memcpy(&got_char, disk + address, sizeof(unsigned char));
// 	printf("int: %i\n", got_int);
// 	printf("char: %c\n", got_char);
// }

// TODO:

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
	// printf("%s\n",(char*)memory);
	for (int x = 0; x < MEMORY_SIZE; x++)
	{
		printf("%u", memory[x]);
		if (x % 4 == 0) printf("\t");
		if (x % 16 == 0) printf("\n%d:", x);
	}
	printf("\n");
}