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
		case 4:
			_print_string();
			break;
		case 8:
			_read_string();
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

void _print_string()
{
	// print string		4	$a0 = address of null-terminated string to print	 
	// todo:
	//	- grab pointer to mem address from $a0
	//	- iterate over each char, if it is 0, break and return
	//  - if it is not, just display that character
	// printf("I'm gonna print a string!");
	int addr = read_register(registers, _$A0);
	int ind = 0;
	// printf("got addrs %d\n", addr);
	char c;
	if (check_flag(f_verbose)||check_flag(f_debug)) printf(ANSI_COLOR_BRIGHT_RED ":" ANSI_COLOR_RESET);
	while(1)
	{
		c = memory[addr + ind];
		ind++;
		if (c == '\\')
		{
			// todo: better way to parse escape chars?
			char c2 = memory[addr + ind];
			if (c2 == 'n')
			{
				c = '\n';
				ind++;
			}
			if (c2 == 't')
			{
				c = '\t';
				ind++;
			}
		}

		printf("%c", c);
		if (c == 0)
		{
			break;
		}
	}
	if (check_flag(f_verbose)||check_flag(f_debug)) printf("\n");
	// free(c);
}

void _read_string()
{	 
	// read string		8	$a0 = address of input buffer
	// 						$a1 = maximum number of characters to read	See note below table
	int in_buff_addr = read_register(registers, _$A0);
	int max_len = read_register(registers, _$A1);

	char* in_string = malloc(max_len + 1);	// todo: change this to not hardcoded
	char format[8];
	sprintf(format, "%%%ds", max_len);
	scanf(format, in_string);
	int wlen = strlen(in_string) + 1;
	for (int wi = 0; wi < wlen; wi++)
	{
		char c = in_string[wi];
		write_memory(&c, memory + in_buff_addr + wi, 1);
	}
	free(in_string);
}

void _print_int()
{
	int to_print = read_register(registers, _$A0);
	if (check_flag(f_verbose)||check_flag(f_debug))
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
	if (check_flag(f_verbose)||check_flag(f_debug)) printf(ANSI_COLOR_BRIGHT_RED "$" ANSI_COLOR_RESET);
	scanf("%d", &in);
	write_register(registers, _$V0, in);
}

void _print_char()
{
	int to_print = read_register(registers, _$A0);
	if (check_flag(f_verbose)||check_flag(f_debug))
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
	if (check_flag(f_verbose)||check_flag(f_debug)) printf(ANSI_COLOR_BRIGHT_RED "$" ANSI_COLOR_RESET);
	// todo: maybe some error handling here? what if we get more than a char?
	scanf("%c", &in);
	write_register(registers, _$V0, (int) in);
}

void _do_terminate_no_code()
{
	exit_info();
	exit(0);
}

void _do_terminate_with_code()
{
	int break_result = read_register(registers, _$A0);
	exit_info();
	exit(break_result);
}