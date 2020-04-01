.data
str1: .asciiz "Writing some values\n"
str2: .asciiz "Reading & Showing some values\n"
comma: .asciiz ", "
nl: .asciiz "\n"
.text
la $a0, str1
li $v0, 4
syscall
# store some ints
li $t0, 64   # starting at address 64
li $t1, 420
sw $t1, 0($t0)
li $t1, 69
sw $t1, 4($t0)
li $t1, 13
sw $t1, 8($t0)
li $t1, 42
sw $t1, 12($t0)
li $t1, 8
sw $t1, 16($t0)
# okay next we want to grab those values and
# display them
la $a0, str2
li $v0, 4
syscall
# print num 1
lw $a0, 0($t0)
li $v0, 1
syscall
la $a0, comma
li $v0, 4
syscall
# print num 2
lw $a0, 4($t0)
li $v0, 1
syscall
la $a0, comma
li $v0, 4
syscall
# print num 3
lw $a0, 8($t0)
li $v0, 1
syscall
la $a0, comma
li $v0, 4
syscall
# print num 4
lw $a0, 12($t0)
li $v0, 1
syscall
la $a0, comma
li $v0, 4
syscall
# print num 5
lw $a0, 16($t0)
li $v0, 1
syscall
la $a0, nl
li $v0, 4
syscall
# exit
li $v0, 10
syscall
