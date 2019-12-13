.data
str1: .asciiz "hello, world\n"
.text
la $t0, str1
li $t1, 72
sw $t1, 0($t0)
lw $t2, 4($t0)