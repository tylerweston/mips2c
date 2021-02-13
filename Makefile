CC=gcc
CFLAGS=-pedantic -Wall -Wextra -Wfloat-equal -std=gnu99 -g
OBJS=mips2c.o mips2c_memory.o mips2c_error.o mips2c_instructions.o\
 mips2c_registers.o mips2c_pseudo_instr.o mips2c_i_instr.o mips2c_j_instr.o mips2c_syscalls.c mips2c_r_instr.o


mips2c:	$(OBJS)
	$(CC) $(OBJS) -o mips2c

mips2c.o: mips2c.c
	$(CC) -O -c mips2c.c

mips2c_error.o: mips2c_error.c
	$(CC) -O -c mips2c_error.c

mips2c_instructions.o: mips2c_instructions.c
	$(CC) -O -c mips2c_instructions.c

mips2c_registers.o: mips2c_registers.c
	$(CC) -O -c mips2c_registers.c

mips2c_i_instr.o: mips2c_i_instr.c
	$(CC) -O -c mips2c_i_instr.c

mips2c_j_instr.o: mips2c_j_instr.c
	$(CC) -O -c mips2c_j_instr.c

mips2c_r_instr.o: mips2c_r_instr.c
	$(CC) -O -c mips2c_r_instr.c

mips2c_pseudo_instr.o: mips2c_pseudo_instr.c
	$(CC) -O -c mips2c_pseudo_instr.c

mips2c_syscalls.o: mips2c_syscalls.c
	$(CC) -O -c mips2c_syscalls.c

mips2c_memory.o: mips2c_memory.c
	$(CC) -O -c mips2c_memory.c