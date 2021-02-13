//mips2c_instructions.h

#ifndef MIPS2C_INSTRUCTIONS
#define MIPS2C_INSTRUCTIONS

typedef enum{
	R_instruction,
	I_instruction,
	J_instruction,
	P_instruction,	// pseudoinstructions
	none
} instruction_type;

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

// instructions
int32_t instruction_to_machine_code(parsed_instruction* p);
parsed_instruction* parse_instruction(char* statement);
void execute_instruction(parsed_instruction* p);
instruction_type get_instruction_type(char* expr);
int32_t get_opcode(char* expr);
int get_num_args(char* expr);
void print_instruction(parsed_instruction* p);
int32_t get_funct(char* expr);
int get_line_from_labels(char* search_label);



// j - type instructions
void _j(char* label);
void _jal(char* label);

// TODO: These shouldn't take specific registers, they should just know where to get this
// information from
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

#endif