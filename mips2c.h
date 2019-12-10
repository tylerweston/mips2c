/*
	mip2c header file
*/

// shared includes

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

// defines

#define MAX_LABEL_LENGTH 16	// max # of chars allowed per label
#define MEMORY_SIZE 128		// 32 * 4 memory!

// macros  for regiters
#define _$0 0
#define _$ZERO 0
#define _$AT 1
#define _$V0 2
#define _$V1 3
#define _$A0 4
#define _$A1 5
#define _$A2 6
#define _$A3 7
#define _$T0 8
#define _$T1 9
#define _$T2 10
#define _$T3 11
#define _$T4 12
#define _$T5 13
#define _$T6 14
#define _$T7 15
#define _$S0 16
#define _$S1 17
#define _$S2 18
#define _$S3 19
#define _$S4 20
#define _$S5 21
#define _$S6 22
#define _$S7 23
#define _$T8 24
#define _$T9 25
#define _$K0 26
#define _$K1 27
#define _$GP 28
#define _$SP 29
#define _$FP 30
#define _$RA 31


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


// #define ANSI_BG_COLOR_BLACK   "\x1b[40m"
// #define ANSI_BG_COLOR_RED     "\x1b[41m"
// #define ANSI_BG_COLOR_GREEN   "\x1b[42m"
// #define ANSI_BG_COLOR_YELLOW  "\x1b[43m"
// #define ANSI_BG_COLOR_BLUE    "\x1b[44m"
// #define ANSI_BG_COLOR_MAGENTA "\x1b[45m"
// #define ANSI_BG_COLOR_CYAN    "\x1b[46m"
// #define ANSI_BG_COLOR_BRIGHT_RED     "\x1b[41;1m"
// #define ANSI_BG_COLOR_BRIGHT_GREEN   "\x1b[42;1m"
// #define ANSI_BG_COLOR_BRIGHT_YELLOW  "\x1b[43;1m"
// #define ANSI_BG_COLOR_BRIGHT_BLUE    "\x1b[44;1m"
// #define ANSI_BG_COLOR_BRIGHT_MAGENTA "\x1b[45;1m"
// #define ANSI_BG_COLOR_BRIGHT_CYAN    "\x1b[46;1m"

// define opcodes
// https://inst.eecs.berkeley.edu/~cs61c/resources/MIPS_help.html

// r-type
// note these represent funct values since they all have opcode 0
#define SLL 0
#define SRL 2
#define SRA 3
#define SLLV 4
#define SRLV 6
#define SRAV 7
#define JR 8
#define JALR 9
#define SYSCALL 12
#define MFHI 16
#define MTHI 17
#define MFLO 18
#define MTLO 19
#define MULT 24
#define MULTU 25
#define DIV 26
#define DIVU 27
#define ADD 32
#define ADDU 33
#define SUB 34
#define SUBU 35
#define AND 36
#define OR 37
#define XOR 38
#define NOR 39
#define SLT 42
#define SLTU 43

// ... j-type
// these are opcodes
#define J 2
#define JAL 3

// ... i-type
// these are opcodes
#define BEQ 4
#define BNE 5
#define BLEZ 6
#define BGTZ 7
#define ADDI 8
#define ADDIU 9
#define SLTI 10
#define SLTIU 11
#define ANDI 12
#define ORI 13
#define XORI 14
#define LUI 15
#define LB 32
#define LH 33
#define LW 34
#define LBU 36
#define LHU 37
#define SB 40
#define SH 41
#define SW 43

#define NONE 999

#define debug(x) if(debug) printf(ANSI_COLOR_BRIGHT_GREEN "Debug" ANSI_COLOR_RESET ": " x "\n")
#define warning(x) if(display_warnings) printf(ANSI_COLOR_CYAN "Warning" ANSI_COLOR_RESET ": " x "\n")

// typedefs
typedef struct {
	char* filename;
	char** source;
	int lines;
} program;

// todo: write instruction parsing stuff
typedef enum{
	R_instruction,
	I_instruction,
	J_instruction,
	P_instruction,	// pseudoinstructions
	none
} instruction_type;

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

typedef struct{
	char* label;
	data_types data_type;
	int source_line;
	char* mem_ptr;
	struct labels_list* next;
} label_list;

typedef struct {
	char* instruction;			// instruction expr
	instruction_type type;		// R, I, J type
	int32_t opcode;				// 6 bit opcode
	int32_t* s_reg;				// pointer to register
	int32_t* t_reg;				// pointer to register
	int32_t* d_reg;				// pointer to register
	int s_reg_no;				// register number
	int t_reg_no;				// register number
	int d_reg_no;				// register number
	int32_t shamt;				// 5 bit shift amount
	int32_t funct;				// 6 bit function
	int32_t imm;				// 16 bit immediate val
	int32_t address;			// 26 bit address (do we still need?)
	char* label;				// label!
} parsed_instruction;


// program counter
int pc;

// todo: MEMORY goes here!
unsigned char* data_segment[MEMORY_SIZE];
int data_segment_offset;

unsigned char* heap[MEMORY_SIZE];
int heap_offset;
unsigned char* stack[MEMORY_SIZE];

// we have 32 32-bit registers and HI, LO
int32_t registers[32];
int32_t LO;
int32_t HI;	

// fpu stuff
int32_t fpu_registers[32];

bool verbose;					// verbose - show program lines as interpreted
bool debug;						// debug mode - display debug info
bool display_instructions;		// show individual parsed instructions
bool display_registers;			// display registers at end
bool display_warnings;
bool display_memory;

label_list* labels;

// functions

program get_program(char* filename);
// int free_program(char** program);
int str_to_int(char* str);	// just in case atoi is a no go
int align4(int num);

// instructions
int32_t instruction_to_machine_code(parsed_instruction* p);
parsed_instruction* parse_instruction(char* statement);
void execute_instruction(parsed_instruction* p);
instruction_type get_instruction_type(char* expr);
int32_t get_opcode(char* expr);
int get_num_args(char* expr);
void print_instruction(parsed_instruction* p);
int32_t get_funct(char* expr);

// register stuff
// todo: we can take all of these references to registers out of here
// since our registers now live in a more global scope than we assumed
// it would!
void print_info();
int32_t write_register(int32_t* registers, int reg_num, int value);
int32_t read_register(int32_t* registers, int reg_num);
char* reg_num_to_str(int reg_num);
int32_t* get_register(char* reg);
int get_register_no(char* reg);
void print_registers(int32_t* registers);

// j - type instructions

void _j(int32_t *pc);
void _jal(int32_t *pc);

// r - type instructions

void _sll(int32_t *d, int32_t* t, int32_t shamt);
void _srl(int32_t *d, int32_t* t, int32_t shamt);
void _sra(int32_t *d, int32_t* t, int32_t shamt);
void _sllv(int32_t *d, int32_t* t, int32_t* s);
void _srlv(int32_t *d, int32_t* t, int32_t* s);
void _srav(int32_t *d, int32_t* t, int32_t* s);
void _syscall();
void _mfhi(int32_t *d);
void _mthi(int32_t *s);
void _mflo(int32_t *d);
void _mtlo(int32_t *s);
void _jr(int32_t* s);
void _jalr(int32_t* d, int32_t *s);
void _mult(int32_t* s, int32_t *t);
void _multu(int32_t* s, int32_t *t);
void _div(int32_t* s, int32_t *t);
void _divu(int32_t* s, int32_t *t);
void _add(int32_t* d, int32_t* s, int32_t* t);
void _addu(int32_t* d, int32_t* s, int32_t* t);
void _sub(int32_t* d, int32_t* s, int32_t* t);
void _subu(int32_t* d, int32_t* s, int32_t* t);
// void _addi(int32_t* t, int32_t* s, int32_t imm);
// void _addiu(int32_t* t, int32_t* s, int32_t imm);
void _and(int32_t *d, int32_t* s, int32_t* t);
void _or(int32_t *d, int32_t* s, int32_t* t);
void _xor(int32_t *d, int32_t* s, int32_t* t);
void _nor(int32_t *d, int32_t* s, int32_t* t);
void _slt(int32_t *d, int32_t* s, int32_t* t);
void _sltu(int32_t *d, int32_t* s, int32_t* t);
void _ori(int32_t *t, int32_t *s, int32_t imm);

// i-type instructions
void _beq(int32_t *s, int32_t* t, char* label);
void _bne(int32_t *s, int32_t* t, char* label);
void _blez(int32_t *s, char* label);
void _bgtz(int32_t *s, char* label);
void _addi(int32_t *t, int32_t *s, int32_t imm);
void _addiu(int32_t *t, int32_t *s, int32_t imm);
void _slti(int32_t *t, int32_t *s, int32_t imm);
void _sltiu(int32_t *t, int32_t *s, int32_t imm);
void _andi(int32_t *t, int32_t *s, int32_t imm);
void _ori(int32_t *t, int32_t *s, int32_t imm);
void _xori(int32_t *t, int32_t *s, int32_t imm);

// Memory

void _lb(int32_t *t, int32_t* s, int32_t offset);
void _lh(int32_t *t, int32_t* s, int32_t offset);
void _lbu(int32_t *t, int32_t* s, int32_t offset);
void _lhu(int32_t *t, int32_t* s, int32_t offset);
void _lw(int32_t *t, int32_t* s, int32_t offset);
void _lui(int32_t *t, int32_t offset);
void _sb(int32_t *t, int32_t* s, int32_t offset);
void _sh(int32_t *t, int32_t* s, int32_t offset);
void _sw(int32_t *t, int32_t* s, int32_t offset);


// pseudoinstructions

void _move(int32_t *t, int32_t* s);
void _clear(int32_t *t);
void _li(int32_t *t, int imm);
void _la(int32_t *d, char* label);
// void _div(int32_t *d, int32_t *s, int32_t *t);
void _rem(int32_t *d, int32_t *s, int32_t *t);
void _not(int32_t *t, int32_t *s);
void _nop();

// syscall handling stuff

void _do_syscall();
void _print_int();
void _get_int();
void _do_terminate_with_code();
void _do_terminate_no_code();
void _print_char();
void _get_char();
void _print_string();
void _read_string();

// Memory stuff
void write_memory(unsigned char *item, unsigned char* mem_loc, int size);
void get_memory(unsigned char *save_loc, unsigned char* mem_loc, int size);
void clear_memory();
void print_memory();

// for debugging purposes
void print_labels();

void error(char* error);