# comment, everything after will be ignored
# first is a .data section
# .data
# declaration of data
# hello_world: .asciiz "Hello World\n"
#num1: .word 42
#arr1: .word 1, 2, 3, 4, 5
#arr2: .byte 'a', 'b'
#buffer: .space 60

##_main:
#addi $t0, $zero, 69
#la $a0, hello_world
#liv $v0, 4
#syscall

ori  $v0, $0, 1           # service 1 is print integer
add $a0, $t0, $zero  # load desired value into arg reg. $a0
syscall

#ori reg, $0, 69 loads value 69 into reg

