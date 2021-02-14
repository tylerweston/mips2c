CC=gcc
CFLAGS=-pedantic -Wall -Wextra -Wfloat-equal -std=gnu99 -g
OBJS=mips2c.o memory.o error.o instructions.o registers.o pseudo_instr.o i_instr.o j_instr.o syscalls.o r_instr.o

mips2c:	$(OBJS)
	$(CC) $(OBJS) -o mips2c

mips2c.o: mips2c.c
	$(CC) -O -c mips2c.c

error.o: error.c
	$(CC) -O -c error.c

instructions.o: instructions.c
	$(CC) -O -c instructions.c

registers.o: registers.c
	$(CC) -O -c registers.c

i_instr.o: i_instr.c
	$(CC) -O -c i_instr.c

j_instr.o: j_instr.c
	$(CC) -O -c j_instr.c

r_instr.o: r_instr.c
	$(CC) -O -c r_instr.c

pseudo_instr.o: pseudo_instr.c
	$(CC) -O -c pseudo_instr.c

syscalls.o: syscalls.c
	$(CC) -O -c syscalls.c

memory.o: memory.c
	$(CC) -O -c memory.c

.PHONY : check
check:
	cd tests; bash run_tests.sh

.PHONY : clean
clean:
	rm $(OBJS) mips2c