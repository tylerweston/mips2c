// mips2c_memory.h

#ifndef MIPS2C_MEMORY
#define MIPS2C_MEMORY

// main memory
extern char* memory[MEMORY_SIZE];

// Memory stuff
void write_memory(char *item, char* mem_loc, int size);
void get_memory(char *save_loc, char* mem_loc, int size);
void clear_memory();
void print_memory();

#endif