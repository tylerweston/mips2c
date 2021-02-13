# hello world
.data
out_string: .asciiz "Hello, World!\n"
inty: .word 420
hvar: .half 12
bvar: .byte 2
.text
main:
#li $v0, 4	#4=syscall to pring string
ori $v0, $0, 4
la $a0, out_string #load address of outstring into reg $a0
syscall
#li $v0, 10	#10=syscall to exit program
ori $v0, $0, 10
syscall