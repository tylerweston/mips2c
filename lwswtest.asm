#lwsw test
.data
tst: .word 0
.text
la $t0, tst
li $t1, 69
sw $t1, 12($t0)
lw $t2, 12($t0)