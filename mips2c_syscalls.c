/*
	mips2c syscalls.c
	- implementations of syscalls
*/

// FUN TO DO: implement RANDOM int so we can get some random going on! maybe some MARS only syscalls?

// MEMORY STUFF TO DO:
// -------------------

// print string		4	$a0 = address of null-terminated string to print	 
// read string		8	$a0 = address of input buffer
// 						$a1 = maximum number of characters to read	See note below table
// sbrk 
//(alloc. heap mem)	9	$a0 = number of bytes to allocate	$v0 contains address of allocated memory
	 
// FPU STUFF TO DO:
// ----------------
// print float		2	$f12 = float to print	 
// print double		3	$f12 = double to print	
// read float		6	$f0 contains float read
// read double		7	$f0 contains double read

// FILE STUFF TO DO:
// -----------------
// open file		13	$a0 = address of null-terminated string containing filename
//						$a1 = flags
// 						$a2 = mode	$v0 contains file descriptor (negative if error). See note below table
// read from file	14	$a0 = file descriptor
// 						$a1 = address of input buffer
// 						$a2 = maximum number of characters to read	$v0 contains number of characters read (0 if end-of-file, negative if error). See note below table
// write to file	15	$a0 = file descriptor
//	 					$a1 = address of output buffer
// 						$a2 = number of characters to write	$v0 contains number of characters written (negative if error). See note below table
// close file		16	$a0 = file descriptor	 

#include "mips2c.h"

void _do_syscall()
{
	// syscall number is stored in $v0
	int call_number = read_register(registers, _$V0);
	switch (call_number)
	{
		case 1:
			_print_int();
			break;
		case 5:
			_get_int();
			break;
		case 10:
			_do_terminate_no_code();
			break;
		case 11:
			_print_char();
			break;
		case 12:
			_get_char();
			break;
		case 17:
			_do_terminate_with_code();
			break;
	}
}

void _print_int()
{
	int to_print = read_register(registers, _$A0);
	if (verbose)
	{
		// a little extra output if we're in verbose mode
		printf(ANSI_COLOR_BRIGHT_RED ":" ANSI_COLOR_RESET "%i", to_print);
		printf("\n");
	}
	else
	{
		printf("%i", to_print);
	}
}

void _get_int()
{
	int in;
	if (verbose) printf(ANSI_COLOR_BRIGHT_RED "$" ANSI_COLOR_RESET);
	scanf("%d", &in);
	write_register(registers, _$V0, in);
}

void _print_char()
{
	int to_print = read_register(registers, _$A0);
	if (verbose)
	{
		// a little extra output if we're in verbose mode	
		printf(ANSI_COLOR_BRIGHT_RED ":" ANSI_COLOR_RESET "%c", to_print);
		printf("\n");
	}
	else
	{
		printf("%c", to_print);		
	}
}

void _get_char()
{
	char in;
	if (verbose) printf(ANSI_COLOR_BRIGHT_RED "$" ANSI_COLOR_RESET);
	// todo: maybe some error handling here? what if we get more than a char?
	scanf("%c", &in);
	write_register(registers, _$V0, (int) in);
}

void _do_terminate_no_code()
{
	if (display_registers) print_registers(registers);
	exit(0);
}

void _do_terminate_with_code()
{
	int break_result = read_register(registers, _$A0);
	if (display_registers) print_registers(registers);
	exit(break_result);
}