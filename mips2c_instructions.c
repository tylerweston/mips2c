/*
	mips2c instructions handler
		- parse instructions
		- return opcodes for functions
		- return instruction type
		- return number of arguments an expression has
*/

#include "mips2c.h"

int32_t instruction_to_machine_code(parsed_instruction* p)
{
	// convert a statement into it's 32bit machine code
	// 6 bits opcode
	// 5 bits rs
	// 5 bits rt
	// 5 bits rd
	// 5 bit shamt
	// 6 bits func
	// switch based on instruction type
	// todo:
	//	- this will all just be masking and bit-shift tomfoolery!
	//	- R INSTRUCTION
	//	- convert opcode to binary
	//	- convert s reg to binary
	//	- convert t reg to binary
	//	- convert d reg to binary
	//	- convert shamt to binary
	//	- convert func to binary
	//	- I/J INSTRUCTION
	//	- todo...
	//	- Concatenate all bits
	//	- return this 32bit binary number!

	// to get distance between labels find our current pc and
	// find the pc to our destination and subtract (??)

}

parsed_instruction* parse_instruction(char* statement)
{
	// Given a statement, parse it to return a parsed_instruction
	if (debug) {
		printf(ANSI_COLOR_BRIGHT_RED "> " ANSI_COLOR_RESET"%s\n", statement);
	}
	// if we ever get a blank statement, throw a warning
	if ((strcmp(statement, " ") == 0) ||
		(statement[0] == 0))
	{
		warning("parse_instruction got blank statement");
		return NULL;
	}

	char* s = strdup(statement);	// make a copy so we don't mess up original
	const char d[2] = " ";

	char *expr;

	// todo: this doesn't currently handle two spaces in a row correctly, fix that
	// todo: do NOT do this with strsep, implement this by hand
	expr = strtok_r(s, d, &s);

	// todo: for now assume lower case
	// expr = lower_case(expr);

	// s register: source
	// t register: target
	// d register: destination

	if (strchr(expr, ':') != NULL) 
	{
		if (verbose)
		{
			printf(ANSI_COLOR_BRIGHT_YELLOW);
			printf("%s " ANSI_COLOR_RESET, expr);
		}

		expr = strtok_r(s, d, &s);
		if (expr == NULL) 
		{
			if (verbose) printf("\n");
			return NULL;
		}
	}

	if (strchr(expr, '.') == NULL && (verbose))
	{
				printf(ANSI_COLOR_BRIGHT_GREEN);
				printf("%s " ANSI_COLOR_RESET, expr);	
	}

	parsed_instruction* p = malloc (sizeof(parsed_instruction) + MAX_LABEL_LENGTH + 5);
	p->instruction = malloc(strlen(expr)+1);
	strcpy(p->instruction, expr);
	p->type = get_instruction_type(expr);
	p->opcode = get_opcode(expr);

	if (p->opcode == 0)
	{
		p->funct = get_funct(expr);
	}
	else
	{
		p->funct = -1;
	}

	// set maybe unused fields
	p->s_reg = NULL;
	p->t_reg = NULL;
	p->d_reg = NULL;
	p->shamt = -1;
	p->imm = -1;
	p->address = -1;
	p->label = malloc(MAX_LABEL_LENGTH);

	// get all required registers
	char* rs;
	char* rt;
	char* rd;
	char* imm;
	// char* shamt;	<-- will need to grab this once we implement shifted stuff
	// char* address; <-- do we need this or just label?

	// todo:
	// we can parse instructions like this
	// get(**part1, **part2, **part3) (??)
	// and then pass pointers to reg as args? maybe also have to
	// specify the type of these parts?
	// or make a struct that contains the type and enough information
	// to tell what we're trying to put there?

	if (verbose)
	{
		if (expr[0] == '.')
		{
			printf(ANSI_COLOR_BRIGHT_CYAN "%s " ANSI_COLOR_RESET, expr);
			printf(ANSI_COLOR_YELLOW "%s\n" ANSI_COLOR_RESET, s);
			return NULL;
		}
	}

	switch (p->type)
	{
		case none:
			if (verbose) printf("\n");
			if (display_warnings) warning("Have none instruction");
			return NULL;

		case P_instruction:
			if (strcmp(expr, "move") == 0 ||
				strcmp(expr, "not") == 0 )
			{
				// two args, t then s
				rt = strtok_r(s, d, &s);
				if (rt == NULL)
				{
					error("Couldn't read register t");
				}
				p->t_reg = get_register(rt);
				p->t_reg_no = get_register_no(rt);
				if (verbose) printf(ANSI_COLOR_MAGENTA "%s " ANSI_COLOR_RESET, rt);

				rs = strtok_r(s, d, &s);
				if (rs == NULL)
				{
					error("Couldn't read register s");
				}
				p->s_reg = get_register(rs);
				p->s_reg_no = get_register_no(rs);
				if (verbose) printf(ANSI_COLOR_MAGENTA "%s " ANSI_COLOR_RESET, rs);
			}

			if (strcmp(expr, "li") == 0 ||
				strcmp(expr, "la") == 0)
			{

				char* toke;

				toke = strtok_r(s, d, &s);
				if (toke == NULL)
				{
					error("Couldn't read register");
				}
				if (strcmp(expr, "li") == 0)
				{
					p->t_reg = get_register(toke);
					p->t_reg_no = get_register_no(toke);	
				}
								
				if (strcmp(expr, "la") == 0)
				{
					p->d_reg = get_register(toke);
					p->d_reg_no = get_register_no(toke);	
				}
				if (verbose) printf(ANSI_COLOR_MAGENTA "%s " ANSI_COLOR_RESET, toke);
			

				char* get_label;
				get_label = strtok_r(s, d, &s);
				
				if (get_label == NULL)
				{
					error("Couldn't read label");
				}

				if (strcmp(expr, "la") == 0)
				{
					strcpy(p->label, get_label);
					if (verbose) printf(ANSI_COLOR_BRIGHT_YELLOW "%s " ANSI_COLOR_RESET, get_label);		
				}

				if (strcmp(expr, "li") == 0)
				{
					p->imm = str_to_int(get_label);
					if (verbose) printf(ANSI_COLOR_CYAN "%s " ANSI_COLOR_RESET, get_label);
				}
			}

			break;

		case R_instruction:

			// todo: how to grab shift amount?
			if (strcmp(expr, "syscall") == 0)
			{
				break;
			}

			// todo: better way to split up these registers!! different instructions assign registers in different ways!
			if (strcmp(expr, "jr") == 0)
			{
				rs = strtok_r(s, d, &s);
				if (rs == NULL)
				{
					error("Couldn't read register s");
				}
				p->s_reg = get_register(rs);
				p->s_reg_no = get_register_no(rs);
				if (verbose) printf(ANSI_COLOR_MAGENTA "%s " ANSI_COLOR_RESET, rs);
				break;
			}

			// todo: error checking here
			if (get_num_args(expr) == 3)
			{
				rd = strtok_r(s, d, &s);
				if (rd == NULL)
				{
					error("Couldn't read register d");
				}
				p->d_reg = get_register(rd);
				p->d_reg_no = get_register_no(rd);
				if (verbose) printf(ANSI_COLOR_MAGENTA "%s " ANSI_COLOR_RESET, rd);
			}

			rs = strtok_r(s, d, &s);
			if (rs == NULL)
			{
				error("Couldn't read register s");
			}
			p->s_reg = get_register(rs);
			p->s_reg_no = get_register_no(rs);
			if (verbose) printf(ANSI_COLOR_MAGENTA "%s " ANSI_COLOR_RESET, rs);

			rt = strtok_r(s, d, &s);
			if (rt == NULL)
			{
				error("Couldn't read register t");
			}
			p->t_reg = get_register(rt);
			p->t_reg_no = get_register_no(rt);
			if (verbose) printf(ANSI_COLOR_MAGENTA "%s " ANSI_COLOR_RESET, rt);
			break;

		case I_instruction:

			if (strcmp(expr, "sw") == 0 ||
				strcmp(expr, "lw") == 0)
			{
				// $t, offset($s)
				rt = strtok_r(s, d, &s);
				if (rt == NULL)
				{
					error("Couldn't read register t");
				}
				p->t_reg = get_register(rt);
				p->t_reg_no = get_register_no(rt);
				if (verbose) printf(ANSI_COLOR_MAGENTA "%s " ANSI_COLOR_RESET, rt);

				if (s[0] == '(')
				{
					// handle case where offset is not specified
					// ie sw $t0, ($t1)
					p->imm = 0;
					s++;
				}
				else
				{
					// handle case where we have an offset
					// ie lw $t0, 40($t1)
					imm = strtok_r(s, "(", &s);
					p->imm = str_to_int(imm);
				}

				if (verbose) printf(ANSI_COLOR_CYAN "%s" ANSI_COLOR_RESET, imm);

				rs = strtok_r(s, ")", &s);
				if (rs == NULL)
				{
					error("Couldn't read register s");
				}

				p->s_reg = get_register(rs);
				p->s_reg_no = get_register_no(rs);
				if (verbose) printf("(");
				if (verbose) printf(ANSI_COLOR_MAGENTA "%s" ANSI_COLOR_RESET, rs);	
				if (verbose) printf(")");
			}
			else
			if (strcmp(expr, "beq") == 0 ||
				strcmp(expr, "bne") == 0 ||
				strcmp(expr, "blez") == 0 ||
				strcmp(expr, "bgtz") == 0)
			{

				rs = strtok_r(s, d, &s);
				if (rs == NULL)
				{
					error("Couldn't read register s");
				}
				p->s_reg = get_register(rs);
				p->s_reg_no = get_register_no(rs);
				if (verbose) printf(ANSI_COLOR_MAGENTA "%s " ANSI_COLOR_RESET, rs);	

				if (get_num_args(expr) == 2)
				{
					rt = strtok_r(s, d, &s);
					if (rt == NULL)
					{
						error("Couldn't read register t");
					}
					p->t_reg = get_register(rt);
					p->t_reg_no = get_register_no(rt);	
					if (verbose) printf(ANSI_COLOR_MAGENTA "%s " ANSI_COLOR_RESET, rt);
				}

				char* get_label;
				get_label = strtok_r(s, d, &s);
				if (get_label == NULL)
				{
					error("Couldn't read label");
				}
				strcpy(p->label, get_label);
				if (verbose) printf(ANSI_COLOR_BRIGHT_YELLOW "%s " ANSI_COLOR_RESET, get_label);	
			} 
			else 
			{
				rt = strtok_r(s, d, &s);
				if (rt == NULL)
				{
					error("Couldn't read register t");
				}
				p->t_reg = get_register(rt);
				p->t_reg_no = get_register_no(rt);			
				if (verbose) printf(ANSI_COLOR_MAGENTA "%s " ANSI_COLOR_RESET, rt);


				if (get_num_args(expr) == 2)
				{
					rs = strtok_r(s, d, &s);
					if (rs == NULL)
					{
						error("Couldn't read register s");
					}
					p->s_reg = get_register(rs);
					p->s_reg_no = get_register_no(rs);
					if (verbose) printf(ANSI_COLOR_MAGENTA "%s " ANSI_COLOR_RESET, rs);
				}

				imm = strtok_r(s, d, &s);
				p->imm = str_to_int(imm);
				if (verbose) printf(ANSI_COLOR_CYAN "%s " ANSI_COLOR_RESET, imm);
			}
			break;

		case J_instruction:
			;	// <- hack, can't have declaration right after label??
			char* get_label;
			get_label = strtok_r(s, d, &s);
			if (get_label == NULL)
			{
				error("Couldn't read label");
			}
			strcpy(p->label, get_label);
			if (verbose) printf(ANSI_COLOR_BRIGHT_YELLOW "%s " ANSI_COLOR_RESET, get_label);	
			break;

		default:
			break;
	}

	if (verbose) printf(" \n");

	return p;
}

void execute_instruction(parsed_instruction* p)
{
	if (display_instructions) print_instruction(p);


	// if (p->type == J_instruction)
	// {
	// 	switch(p->opcode)
	// 	{
	// 		case J:
	// 			_j(p->label);
	// 			return;
	// 		case JAL:
	// 			_jal(p->label);
	// 			return;
	// 	}
	// }

	if (p->type == P_instruction)
	{
		if (strcmp(p->instruction, "li") == 0)
		{
			_li(p->t_reg, p->imm);
			return;
		}

		if (strcmp(p->instruction, "la") == 0)
		{
			_la(p->d_reg, p->label);
			return;
		}

		if (strcmp(p->instruction, "clear") == 0)
		{
			_clear(p->t_reg);
			return;
		}

		if (strcmp(p->instruction, "nop") == 0)
		{
			_nop();	// don't really need this but just in case
			return;
		}

		if (strcmp(p->instruction, "move") == 0)
		{
			_move(p->t_reg, p->s_reg);
			return;
		}

		if (strcmp(p->instruction, "not") == 0)
		{
			_not(p->t_reg, p->s_reg);
			return;
		}

		// todo: rest of pseudoinstructions herer!

	}

	if (p->opcode == 0)
	{
		// executing r-type instructions
		switch (p->funct)
		{
			case SLL:
				_sll(p->d_reg, p->t_reg, p->shamt);
				break;
			case SRL:
				_srl(p->d_reg, p->t_reg, p->shamt);
				break;
			case SRA:
				_sra(p->d_reg, p->t_reg, p->shamt);
				break;
			case SLLV:
				_sllv(p->d_reg, p->t_reg, p->s_reg);
				break;
			case SRLV:
				_srlv(p->d_reg, p->t_reg, p->s_reg);
				break;
			case SRAV:
				_srav(p->d_reg, p->t_reg, p->s_reg);
				break;
			case JR:
				_jr(p->s_reg);
				break;
			case JALR:		// todo: there is another type of jalr
				_jalr(p->d_reg, p->s_reg);
				break;
			// case JALR_:		// todo: ONE argument call
			// 	_jalr_(p->s_reg);
			// 	break;
			case SYSCALL:
				_do_syscall();
				break;
			case MFHI:
				_mfhi(p->d_reg);
				break;
			case MTHI:
				_mthi(p->s_reg);
				break;
			case MFLO:
				_mflo(p->d_reg);
				break;
			case MTLO:
				_mtlo(p->s_reg);
				break;
			case MULT:
				_mult(p->s_reg, p->t_reg);
				break;
			case MULTU:
				_multu(p->s_reg, p->t_reg);
				break;
			case ADD:
				_add(p->d_reg, p->s_reg, p->t_reg);
				break;
			case SUB:
				_sub(p->d_reg, p->s_reg, p->t_reg);
				break;
		}
	}	
	else
	{
		// executing j/i type instructions
		switch (p->opcode)
		{
			case J:
				_j(p->label);
				break;
			case JAL:
				_jal(p->label);
				break;
			case BEQ:
				_beq(p->s_reg, p->t_reg, p->label);
				break;
			case BNE:
				_bne(p->s_reg, p->t_reg, p->label);
				break;
			case BLEZ:
				_blez(p->s_reg, p->label);
				break;
			case BGTZ:
				_bgtz(p->s_reg, p->label);
				break;
			case ADDI:
				_addi(p->t_reg, p->s_reg, p->imm);
				break;
			case ADDIU:
				_addiu(p->t_reg, p->s_reg, p->imm);
				break;
			case SLTI:
				_slti(p->t_reg, p->s_reg, p->imm);
				break;
			case SLTIU:
				_sltiu(p->t_reg, p->s_reg, p->imm);
				break;
			case ANDI:
				_andi(p->t_reg, p->s_reg, p->imm);
				break;
			case ORI:
				_ori(p->t_reg, p->s_reg, p->imm);
				break;
			case XORI:
				_xori(p->t_reg, p->s_reg, p->imm);
				break;
			case LUI:
				_lui(p->t_reg, p->imm);
				break;
			case LB:
				_lb(p->t_reg, p->s_reg, p->imm);
				break;
			case LH:
				_lh(p->t_reg, p->s_reg, p->imm);
				break;
			case LW:
				_lw(p->t_reg, p->s_reg, p->imm);
				break;
			case LBU:
				_lbu(p->t_reg, p->s_reg, p->imm);
				break;
			case LHU:
				_lhu(p->t_reg, p->s_reg, p->imm);
				break;
			case SB:
				_sb(p->t_reg, p->s_reg, p->imm);
				break;
			case SH:
				_sh(p->t_reg, p->s_reg, p->imm);
				break;
			case SW:
				_sw(p->t_reg, p->s_reg, p->imm);
				break;
		}
	}
}


instruction_type get_instruction_type(char* expr)
{
	// given an expression, return the instruction type

	if (strcmp(expr, "add") == 0 ||
		strcmp(expr, "syscall") == 0 ||
		strcmp(expr, "sll") == 0 ||
		strcmp(expr, "srl") == 0 ||
		strcmp(expr, "sra") == 0 ||
		strcmp(expr, "sllv") == 0 ||
		strcmp(expr, "srlv") == 0 ||
		strcmp(expr, "srav") == 0 ||
		strcmp(expr, "jr") == 0 ||
		strcmp(expr, "jalr") == 0 ||
		strcmp(expr, "mfhi") == 0 ||
		strcmp(expr, "mthi") == 0 ||
		strcmp(expr, "mflo") == 0 ||
		strcmp(expr, "mtlo") == 0 ||
		strcmp(expr, "mult") == 0 ||
		strcmp(expr, "multu") == 0 ||
		strcmp(expr, "div") == 0 ||
		strcmp(expr, "divu") == 0 ||
		strcmp(expr, "addu") == 0 ||
		strcmp(expr, "sub") == 0 ||
		strcmp(expr, "subu") == 0 ||
		strcmp(expr, "and") == 0 ||
		strcmp(expr, "or") == 0 ||
		strcmp(expr, "xor") == 0 ||
		strcmp(expr, "nor") == 0 ||
		strcmp(expr, "slt") == 0 ||
		strcmp(expr, "sltu") == 0) 
	{
		return R_instruction;
	}

	if (strcmp(expr, "beq") == 0 ||
		strcmp(expr, "bne") == 0 ||
		strcmp(expr, "blez") == 0 ||
		strcmp(expr, "bgtz") == 0 ||
		strcmp(expr, "addi") == 0 ||
		strcmp(expr, "addiu") == 0 ||
		strcmp(expr, "slti") == 0 ||
		strcmp(expr, "sltiu") == 0 ||
		strcmp(expr, "andi") == 0 ||
		strcmp(expr, "ori") == 0 ||
		strcmp(expr, "xori") == 0 ||
		strcmp(expr, "lui") == 0 ||
		strcmp(expr, "lb") == 0 ||
		strcmp(expr, "lh") == 0 ||
		strcmp(expr, "lw") == 0 ||
		strcmp(expr, "lbu") == 0 ||
		strcmp(expr, "lhu") == 0 ||
		strcmp(expr, "sb") == 0 ||
		strcmp(expr, "sh") == 0 ||
		strcmp(expr, "sw") == 0)
	{
		return I_instruction;
	}

	if (strcmp(expr, "j") == 0 ||
		strcmp(expr, "jal") == 0)
	{
		return J_instruction;
	}

	if (strcmp(expr, "b") == 0 ||
		strcmp(expr, "abs") == 0 ||
		strcmp(expr, "blt") == 0 ||
		strcmp(expr, "bgt") == 0 ||
		strcmp(expr, "ble") == 0 ||
		strcmp(expr, "neg") == 0 ||
		strcmp(expr, "not") == 0 ||
		strcmp(expr, "bge") == 0 ||
		strcmp(expr, "li") == 0 ||
		strcmp(expr, "la") == 0 ||
		strcmp(expr, "move") == 0 ||
		strcmp(expr, "sge") == 0 ||
		strcmp(expr, "sgt") == 0)
	{
		// should we preprocess this or use it as pseudoinstructions?
		return P_instruction;
	}
	return none;

}

int get_funct(char* expr)
{
	// return function number for r-instructions
	if (strcmp(expr, "add") == 0)
	{
		return ADD;
	}

	if (strcmp(expr, "syscall") == 0)
	{
		return SYSCALL;
	}

	if (strcmp(expr, "sll") == 0)
	{
		return SLL;
	}

	if (strcmp(expr, "srl") == 0) 
	{
		return SRL;
	}

	if (strcmp(expr, "mult") == 0)
	{
		return MULT;
	}

	if (strcmp(expr, "sub") == 0)
	{
		return SUB;
	}

	if (strcmp(expr, "and") == 0)
	{
		return AND;
	}

	if (strcmp(expr, "or") == 0)
	{
		return OR;
	}

	if (strcmp(expr, "xor") == 0)
	{
		return XOR;
	}

	if (strcmp(expr, "nor") == 0)
	{
		return NOR;
	}

	if (strcmp(expr, "div") == 0)
	{
		return DIV;
	}
// todo: function # still

	if (strcmp(expr, "slt") == 0)
	{
		return SLT;
	}

	if (strcmp(expr,"sra") == 0)
	{
		return SRA;
	}

	if (strcmp(expr,"sllv") == 0)
	{
		return SLLV;
	}

	if (strcmp(expr,"srlv") == 0)
	{
		return SRLV;
	}

	if (strcmp(expr,"srav") == 0)
	{
		return SRAV;
	}

	if (strcmp(expr,"jr") == 0)
	{
		return JR;
	}

	if (strcmp(expr,"jal") == 0)
	{
		return JAL;
	}

	if (strcmp(expr,"jalr") == 0)
	{
		return JALR;
	}

	if (strcmp(expr,"mfhi") == 0)
	{
		return MFHI;
	}

	if (strcmp(expr,"mthi") == 0)
	{
		return MTHI;
	}

	if (strcmp(expr,"mflo") == 0)
	{
		return MFLO;
	}

	if (strcmp(expr,"mtlo") == 0)
	{
		return MTLO;
	}

	if (strcmp(expr,"multu") == 0)
	{
		return MULTU;
	}

	if (strcmp(expr,"div") == 0)
	{
		return DIV;
	}

	if (strcmp(expr,"divu") == 0)
	{
		return DIVU;
	}

	if (strcmp(expr,"addu") == 0)
	{
		return ADDU;
	}

	if (strcmp(expr,"subu") == 0)
	{
		return SUBU;
	}

// return SLTU;
	return NONE;	// error OPCODE, something went wrong!

}

int get_opcode(char* expr)
{

	if (strcmp(expr, "add") == 0 ||
		strcmp(expr, "syscall") == 0 ||
		strcmp(expr, "sll") == 0 ||
		strcmp(expr, "srl") == 0 ||
		strcmp(expr, "sra") == 0 ||
		strcmp(expr, "sllv") == 0 ||
		strcmp(expr, "srlv") == 0 ||
		strcmp(expr, "srav") == 0 ||
		strcmp(expr, "jr") == 0 ||
		strcmp(expr, "jalr") == 0 ||
		strcmp(expr, "mfhi") == 0 ||
		strcmp(expr, "mthi") == 0 ||
		strcmp(expr, "mflo") == 0 ||
		strcmp(expr, "mtlo") == 0 ||
		strcmp(expr, "mult") == 0 ||
		strcmp(expr, "multu") == 0 ||
		strcmp(expr, "div") == 0 ||
		strcmp(expr, "divu") == 0 ||
		strcmp(expr, "addu") == 0 ||
		strcmp(expr, "sub") == 0 ||
		strcmp(expr, "subu") == 0 ||
		strcmp(expr, "and") == 0 ||
		strcmp(expr, "or") == 0 ||
		strcmp(expr, "xor") == 0 ||
		strcmp(expr, "nor") == 0 ||
		strcmp(expr, "slt") == 0 ||
		strcmp(expr, "sltu") == 0) 
	{
		return 0;
	}

	// // R-type instrutions


	// I-type instructions

	if (strcmp(expr, "beq") == 0)
	{
		return BEQ; 
	}

	if (strcmp(expr, "bne") == 0)
	{
		return BNE; 
	}

	if (strcmp(expr, "blez") == 0)
	{
		return BLEZ; 
	}

	if (strcmp(expr, "bgtz") == 0)
	{
		return BGTZ; 
	}

	if (strcmp(expr, "addi") == 0)
	{
		return ADDI; 
	}

	if (strcmp(expr, "addiu") == 0)
	{
		return ADDIU; 
	}

	if (strcmp(expr, "slti") == 0)
	{
		return SLTI; 
	}

	if (strcmp(expr, "sltiu") == 0)
	{
		return SLTIU; 
	}

	if (strcmp(expr, "andi") == 0)
	{
		return ANDI; 
	}

	if (strcmp(expr, "ori") == 0)
	{
		return ORI; 
	}

	if (strcmp(expr, "xori") == 0)
	{
		return XORI; 
	}

	if (strcmp(expr, "lui") == 0)
	{
		return LUI; 
	}

	if (strcmp(expr, "lb") == 0)
	{
		return LB; 
	}

	if (strcmp(expr, "lh") == 0)
	{
		return LH; 
	}

	if (strcmp(expr, "lw") == 0)
	{
		return LW;
	}

	if (strcmp(expr, "lbu") == 0)
	{ 
		return LBU; 
	}

	if (strcmp(expr, "lhu") == 0)
	{
		return LHU; 
	}

	if (strcmp(expr, "sb") == 0)
	{
		return SB; 
	}

	if (strcmp(expr, "sh") == 0)
	{
		return SH; 
	}

	if (strcmp(expr, "sw") == 0)
	{
		return SW; 
	}

	if (strcmp(expr, "jal") == 0)
	{
		return JAL;
	}

	if (strcmp(expr, "j") == 0)
	{
		return J;
	}

	return NONE;
}

int get_num_args(char* expr)
{
	// given an instruction, return the number of expected
	// arguments

	// Can we do something like make a dictionary here or something like that?

	// r-type instructions
	if (strcmp(expr, "sll") == 0)
	{
		// also will take shamt
		return 2;
	}

	if (strcmp(expr, "srl") == 0)
	{
		// also will take shamt
		return 2;
	}

	if (strcmp(expr, "sra") == 0)
	{
		// also will take shamt
		return 2;
	}

	if (strcmp(expr, "sllv") == 0)
	{
		return 3;
	}

	if (strcmp(expr, "srlv") == 0)
	{
		return 3;
	}

	if (strcmp(expr, "srav") == 0)
	{
		return 3;
	}
	
	if (strcmp(expr, "jr") == 0)
	{
		return 1;
	}

	if (strcmp(expr, "jalr") == 0)
	{
		return 2;
	}

	if (strcmp(expr, "syscall") == 0)
	{
		return 0;
	}

	if (strcmp(expr, "mfhi") == 0)
	{
		return 1;
	}

	if (strcmp(expr, "mthi") == 0)
	{
		return 1;
	}

	if (strcmp(expr, "mflo") == 0)
	{
		return 1;
	}

	if (strcmp(expr, "mtlo") == 0)
	{
		return 1;
	}

	if (strcmp(expr, "mult") == 0)
	{
		return 2;
	}

	if (strcmp(expr, "multu") == 0)
	{
		return 2;
	}

	if (strcmp(expr, "div") == 0)
	{
		return 2;
	}

	if (strcmp(expr, "divu") == 0)
	{
		return 2;
	}

	if (strcmp(expr,"add") == 0)
	{
		return 3;
	}

	if (strcmp(expr,"addu") == 0)
	{
		return 3;
	}

	if (strcmp(expr,"sub") == 0)
	{
		return 3;
	}

	if (strcmp(expr,"subu") == 0)
	{
		return 3;
	}
	if (strcmp(expr,"and") == 0)
	{
		return 3;
	}

	if (strcmp(expr,"or") == 0)
	{
		return 3;
	}

	if (strcmp(expr,"xor") == 0)
	{
		return 3;
	}

	if (strcmp(expr,"nor") == 0)
	{
		return 3;
	}

	if (strcmp(expr,"slt") == 0)
	{
		return 3;
	}

	if (strcmp(expr,"sltu") == 0)
	{
		return 3;
	}

	// i-type instructions

	if (strcmp(expr, "addi") == 0)
	{
		return 2;
	}

	if (strcmp(expr, "addiu") == 0)
	{
		return 2;
	}

	if (strcmp(expr, "beq") == 0)
	{
		return 2;
	}

	if (strcmp(expr, "bne") == 0)
	{
		return 2;
	}

	if (strcmp(expr, "blez") == 0)
	{
		return 1;
	}

	if (strcmp(expr, "bgtz") == 0)
	{
		return 1;
	}

	if (strcmp(expr, "andi") == 0)
	{
		return 2;
	}

	if (strcmp(expr, "ori") == 0)
	{
		return 2;
	}

	if (strcmp(expr, "xori") == 0)
	{
		return 2;
	}

	if (strcmp(expr, "slti") == 0)
	{
		return 2;
	}

	if (strcmp(expr, "sltiu") == 0)
	{
		return 2;
	}

	if (strcmp(expr, "lui") == 0)
	{
		return 1;
	}	

	if (strcmp(expr, "lb") == 0)
	{
		return 1;
	}	

	if (strcmp(expr, "lh") == 0)
	{
		return 1;
	}	

	if (strcmp(expr, "lw") == 0)
	{
		return 1;
	}	

	if (strcmp(expr, "lbu") == 0)
	{
		return 1;
	}	

	if (strcmp(expr, "lhu") == 0)
	{
		return 1;
	}	

	if (strcmp(expr, "sb") == 0)
	{
		return 1;
	}

	if (strcmp(expr, "sh") == 0)
	{
		return 1;
	}	

	if (strcmp(expr, "sw") == 0)
	{
		return 1;
	}	

	return -1;	// error!

}

void print_instruction(parsed_instruction* p) {
	
	// display information about a parsed instruction!

	printf("------------------------------\n");
	printf("instruction name: \t%s\n", p->instruction);
	printf("Instruction type: \t");

	switch(p->type)
	{
		case R_instruction:
			printf("R instruction\n");
			break;
		case I_instruction:
			printf("I instruction\n");
			break;
		case J_instruction:
			printf("J instruction\n");
			break;
		case P_instruction:
			printf("pseudoinstruction\n");
			break;
		case none:
			printf("none\n");
			break;
	}

	printf("opcode: \t%d\n", p->opcode);
	printf("S register: \t%s / %d\n", reg_num_to_str(p->s_reg_no), p->s_reg_no);
	printf("T register: \t%s / %d\n", reg_num_to_str(p->t_reg_no), p->t_reg_no);
	printf("D register: \t%s / %d\n", reg_num_to_str(p->d_reg_no), p->d_reg_no);	
	printf("shamt: \t%d\n", p->shamt);
	printf("funct: \t%d\n", p->funct);
	printf("immediate: \t%d\n", p->imm);
	printf("address: \t%d\n", p->address);
	printf("------------------------------\n");

}

// helper functions here!

int get_line_from_labels(char* search_label)
{
	if (debug) print_labels();
	label_list *curr = labels;
	while (curr != NULL)
	{
		if (strcmp(curr->label, search_label) == 0)
		{
			return curr->source_line;
		}

		curr = curr->next;
	}

	char err_msg[128];
	sprintf(err_msg, "Cannot find label %s", search_label);
	error(err_msg);
}

