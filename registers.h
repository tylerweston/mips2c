// mips2c registers.h
#ifndef MIPS2C_REGISTER_H
#define MIPS2C_REGISTER_H

// we have 32 32-bit registers and HI, LO
extern int32_t registers[32];
extern int32_t LO;
extern int32_t HI;	

// macros  for registers
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

// register stuff
// todo: we can take all of these references to registers out of here
// since our registers now live in a more global scope than we assumed
// it would! (might be in bad taste to do this though?)
void print_info();
int32_t write_register(int32_t* registers, int reg_num, int value);
int32_t read_register(int32_t* registers, int reg_num);
char* reg_num_to_str(int reg_num);
int32_t* get_register(char* reg);
int get_register_no(char* reg);
void print_registers(int32_t* registers);

#endif