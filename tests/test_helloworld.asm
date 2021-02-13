# hello world
.data
out_string: .asciiz "Hello, World!\n"
.text
main:
ori $v0, $0, 4
la $a0, out_string #load address of outstring into reg $a0
syscall
li $v0, 10	#10=syscall to exit program
syscall