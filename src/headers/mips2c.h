/*
	mip2c header file
*/

// shared includes
#ifndef MIPS2C_H
#define MIPS2C_H

// #include <stdio.h>
#include <ncurses.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

// defines

#define STR_EQ(x, y) (strcmp(x, y) == 0)

#define f_verbose (1)
#define f_debug (1 << 1)
#define f_display_instructions (1 << 2)
#define f_display_registers (1 << 3)
#define f_display_warnings (1 << 4)
#define f_display_memory (1 << 5)
#define f_display_step_number (1 << 6)
#define f_break_max (1 << 7)
#define f_print_stack_pointer (1 << 8)
#define f_no_additional_output (1 << 9)
#define f_curses (1 << 10)
#define f_show_version (1 << 11)
#define f_show_usage (1 << 12)

// does this work?
#define set_flag(x) (flags |= x)
#define check_flag(x) (flags & x)
#define clear_flag(x) (flags &= ~(x))

// tweak these if needed
#define MAX_LABEL_LENGTH 32	// max # of chars allowed per label //should this be diff for strings now?
#define MAX_STR_LENGTH 64
#define MAX_LINE_LENGTH 128
#define MEMORY_SIZE 512		// size in bytes

// color codes for nicer output
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_BRIGHT_RED     "\x1b[31;1m"
#define ANSI_COLOR_BRIGHT_GREEN   "\x1b[32;1m"
#define ANSI_COLOR_BRIGHT_YELLOW  "\x1b[33;1m"
#define ANSI_COLOR_BRIGHT_BLUE    "\x1b[34;1m"
#define ANSI_COLOR_BRIGHT_MAGENTA "\x1b[35;1m"
#define ANSI_COLOR_BRIGHT_CYAN    "\x1b[36;1m"
#define ANSI_COLOR_RESET   "\x1b[0m"


#define debug(x) if(check_flag(f_debug)) printf(ANSI_COLOR_BRIGHT_GREEN "Debug" ANSI_COLOR_RESET ": " x "\n")
#define warning(x) if(check_flag(f_display_warnings)) printf(ANSI_COLOR_CYAN "Warning" ANSI_COLOR_RESET ": " x "\n")

#define MAJOR_VERSION 0
#define MINOR_VERSION 2

// typedefs ===============================================
typedef struct {
	char* filename;
	char** source;
	int lines;
} program;

typedef enum{
	_PC,
	_ASCII,
	_ASCIIZ,
	_BYTE,
	_HALF,	//halfword
	_WORD,
	_FLOAT,
	_DOUBLE,
	_SPACE
} data_types;

typedef enum{
	DATA_STATE,
	TEXT_STATE,
	GLOBAL_STATE
} parser_state;

typedef struct label_list{
	char* label;
	data_types data_type;
	int source_line;
	int mem_address;
	struct label_list* next;
} label_list;

extern int flags;						// global flags

// program counter
extern int pc;				// this should also be int32_t
extern int step_number;
extern int max_steps;

// main memory
extern char* memory[MEMORY_SIZE];

// we have 32 32-bit registers and HI, LO
extern int32_t registers[32];
extern int32_t LO;
extern int32_t HI;	

// fpu stuff
extern int32_t fpu_registers[32];	// does FU registers hold floats?

// address and variable labels
extern label_list* labels;


// functions ========================================================
void PRINT( const char * format, ... );
void single_exitpoint(int status);
program get_program(char* filename);
// int free_program(char** program);
int str_to_int(char* str);	// just in case atoi is a no go
void exit_info();
int align4(int num);
void error(char* error);

// for debugging purposes
void print_labels();

#endif