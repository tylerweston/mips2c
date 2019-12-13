/*
	mips2c register functions
*/
#include "mips2c.h"




void print_info()
{
// todo: print some help info about registers if needed
// taken from https://en.wikibooks.org/wiki/MIPS_Assembly/MIPS_Details
// Number	Name	Comments
// $0	$zero	Always zero
// $1	$at	Reserved for assembler
// $2, $3	$v0, $v1	First and second return values, respectively
// $4, ..., $7	$a0, ..., $a3	First four arguments to functions
// $8, ..., $15	$t0, ..., $t7	Temporary registers
// $16, ..., $23	$s0, ..., $s7	Saved registers
// $24, $25	$t8, $t9	More temporary registers
// $26, $27	$k0, $k1	Reserved for kernel (operating system)
// $28	$gp	Global pointer
// $29	$sp	Stack pointer
// $30	$fp	Frame pointer
// $31	$ra	Return address
}

bool is_reg(char* reg) {
	// check if reg is a valid register
	return get_register_no(reg) == -1;
}

int write_register(int32_t* registers, int reg_num, int value)
{

	if (reg_num == 0 || reg_num == 26 || reg_num == 27 ||
		reg_num == 1 || reg_num == 28 || reg_num == 31 ||
		reg_num == 30 || reg_num == 29)
	{
		// printf("Error: Trying to overwrite register %s\n", reg_num_to_str(reg_num));
		// exit(1);
		error("Trying to overwrite invalid register!");
	}

	registers[reg_num] = value;
	return 1;
}

int32_t read_register(int32_t* registers, int reg_num)
{
	return registers[reg_num];
}

char* reg_num_to_str(int reg_num)
{
	switch(reg_num)
	{
		case 0:
			return "$zero";
			break;
		case 1:
			return "$at";
			break;
		case 2:
			return "$v0";
			break;
		case 3:
			return "$v1";
			break;
		case 4:
			return "$a0";
			break;
		case 5:
			return "$a1";
			break;
		case 6:
			return "$a2";
			break;
		case 7:
			return "$a3";
			break;
		case 8:
			return "$t0";
			break;
		case 9:
			return "$t1";
			break;
		case 10:
			return "$t2";
			break;
		case 11:
			return "$t3";
			break;
		case 12:
			return "$t4";
			break;
		case 13:
			return "$t5";
			break;
		case 14:
			return "$t6";
			break;
		case 15:
			return "$t7";
			break;
		case 16:
			return "$s0";
			break;
		case 17:
			return "$s1";
			break;
		case 18:
			return "$s2";
			break;
		case 19:
			return "$s3";
			break;
		case 20:
			return "$s4";
			break;
		case 21:
			return "$s5";
			break;
		case 22:
			return "$s6";
			break;
		case 23:
			return "$s7";
			break;
		case 24:
			return "$t8";
			break;
		case 25:
			return "$t9";
			break;
		case 26:
			return "$k0";
			break;
		case 27:
			return "$k1";
			break;
		case 28:
			return "$gp";
			break;
		case 29:
			return "$sp";
			break;
		case 30:
			return "$fp";
			break;
		case 31:
			return "$ra";
			break;
		default:
			return "$wrong";
			break;
	}
}

void print_registers(int32_t* registers)
{
	// Display all registers

	printf("Registers:\n");
	printf("----------\n");
	for (int i = 0; i < 32; i++) {
		printf(ANSI_COLOR_MAGENTA "%s " ANSI_COLOR_RESET ": %d  \t\t", reg_num_to_str(i), read_register(registers, i));
		if (i%3 == 0) printf("\n");
	}
	printf("$HI : %d  \t\t$LO : %d", HI, LO);
	printf("\n");

}

int32_t* get_register(char* reg)
{
	// Given a register as a string, returns pointer to reg #

	
	if (strcmp(reg, "$zero") == 0 ||
		strcmp(reg, "$0") == 0)
	{
		return &registers[0];
	}

	if (strcmp(reg, "$at") == 0)
	{
		return &registers[1];
	}

	if (strcmp(reg, "$v0") == 0)
	{
		return &registers[2];
	}

	if (strcmp(reg, "$v1") == 0)
	{
		return &registers[3];
	}

	if (strcmp(reg, "$a0") == 0)
	{
		return &registers[4];
	}

	if (strcmp(reg, "$a1") == 0)
	{
		return &registers[5];
	}

	if (strcmp(reg, "$a2") == 0)
	{
		return &registers[6];
	}

	if (strcmp(reg, "$a3") == 0)
	{
		return &registers[7];
	}

	if (strcmp(reg, "$t0") == 0)
	{
		return &registers[8];
	}

	if (strcmp(reg, "$t1") == 0)
	{
		return &registers[9];
	}

	if (strcmp(reg, "$t2") == 0)
	{
		return &registers[10];
	}

	if (strcmp(reg, "$t3") == 0)
	{
		return &registers[11];
	}

	if (strcmp(reg, "$t4") == 0)
	{
		return &registers[12];
	}

	if (strcmp(reg, "$t5") == 0)
	{
		return &registers[13];
	}

	if (strcmp(reg, "$t6") == 0)
	{
		return &registers[14];
	}

	if (strcmp(reg, "$t7") == 0)
	{
		return &registers[15];
	}

	if (strcmp(reg, "$s0") == 0)
	{
		return &registers[16];
	}

	if (strcmp(reg, "$s1") == 0)
	{
		return &registers[17];
	}

	if (strcmp(reg, "$s2") == 0)
	{
		return &registers[18];
	}

	if (strcmp(reg, "$s3") == 0)
	{
		return &registers[19];
	}

	if (strcmp(reg, "$s4") == 0)
	{
		return &registers[20];
	}

	if (strcmp(reg, "$s5") == 0)
	{
		return &registers[21];
	}

	if (strcmp(reg, "$s6") == 0)
	{
		return &registers[22];
	}

	if (strcmp(reg, "$s7") == 0)
	{
		return &registers[23];
	}

	if (strcmp(reg, "$t8") == 0)
	{
		return &registers[24];
	}

	if (strcmp(reg, "$t9") == 0)
	{
		return &registers[25];
	}

	if (strcmp(reg, "$k0") == 0)
	{
		return &registers[26];
	}

	if (strcmp(reg, "$k1") == 0)
	{
		return &registers[27];
	}

	if (strcmp(reg, "$gp") == 0)
	{
		return &registers[28];
	}

	if (strcmp(reg, "$sp") == 0)
	{
		return &registers[29];
	}

	if (strcmp(reg, "$fp") == 0)
	{
		return &registers[30];
	}

	if (strcmp(reg, "$ra") == 0)
	{
		return &registers[31];
	}

	// if we get here, there is an error with register type
	return NULL;

}

int get_register_no(char* reg)
{
	// Given a register as a string, returns pointer to reg #

	
	if (strcmp(reg, "$zero") == 0 ||
		strcmp(reg, "$0") == 0)
	{
		return 0;
	}

	if (strcmp(reg, "$at") == 0)
	{
		return 1;
	}

	if (strcmp(reg, "$v0") == 0)
	{
		return 2;
	}

	if (strcmp(reg, "$v1") == 0)
	{
		return 3;
	}

	if (strcmp(reg, "$a0") == 0)
	{
		return 4;
	}

	if (strcmp(reg, "$a1") == 0)
	{
		return 5;
	}

	if (strcmp(reg, "$a2") == 0)
	{
		return 6;
	}

	if (strcmp(reg, "$a3") == 0)
	{
		return 7;
	}

	if (strcmp(reg, "$t0") == 0)
	{
		return 8;
	}

	if (strcmp(reg, "$t1") == 0)
	{
		return 9;
	}

	if (strcmp(reg, "$t2") == 0)
	{
		return 10;
	}

	if (strcmp(reg, "$t3") == 0)
	{
		return 11;
	}

	if (strcmp(reg, "$t4") == 0)
	{
		return 12;
	}

	if (strcmp(reg, "$t5") == 0)
	{
		return 13;
	}

	if (strcmp(reg, "$t6") == 0)
	{
		return 14;
	}

	if (strcmp(reg, "$t7") == 0)
	{
		return 15;
	}

	if (strcmp(reg, "$s0") == 0)
	{
		return 16;
	}

	if (strcmp(reg, "$s1") == 0)
	{
		return 17;
	}

	if (strcmp(reg, "$s2") == 0)
	{
		return 18;
	}

	if (strcmp(reg, "$s3") == 0)
	{
		return 19;
	}

	if (strcmp(reg, "$s4") == 0)
	{
		return 20;
	}

	if (strcmp(reg, "$s5") == 0)
	{
		return 21;
	}

	if (strcmp(reg, "$s6") == 0)
	{
		return 22;
	}

	if (strcmp(reg, "$s7") == 0)
	{
		return 23;
	}

	if (strcmp(reg, "$t8") == 0)
	{
		return 24;
	}

	if (strcmp(reg, "$t9") == 0)
	{
		return 25;
	}

	if (strcmp(reg, "$k0") == 0)
	{
		return 26;
	}

	if (strcmp(reg, "$k1") == 0)
	{
		return 27;
	}

	if (strcmp(reg, "$gp") == 0)
	{
		return 28;
	}

	if (strcmp(reg, "$sp") == 0)
	{
		return 29;
	}

	if (strcmp(reg, "$fp") == 0)
	{
		return 30;
	}

	if (strcmp(reg, "$ra") == 0)
	{
		return 31;
	}

	// if we get here, there is an error with register type
	return -1;

}


// registers:

// MIPS registers
// register 	assembly name 	Comment
// r0 			$zero 			Always 0
// r1 			$at 			Reserved for assembler
// r2-r3 		$v0-$v1 		Stores results
// r4-r7 		$a0-$a3 		Stores arguments
// r8-r15 		$t0-$t7 		Temporaries, not saved
// r16-r23 		$s0-$s7 		Contents saved for use later
// r24-r25 		$t8-$t9			More temporaries, not saved
// r26-r27 		$k0-$k1 		Reserved by operating system
// r28 			$gp 			Global pointer
// r29 			$sp 			Stack pointer
// r30 			$fp 			Frame pointer
// r31 			$ra 			Return address
