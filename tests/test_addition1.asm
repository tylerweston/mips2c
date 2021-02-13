# Test some basic math
li $t0, 5
li $t1, 7
add $t0, $t0, $t1
li $v0, 1
move $a0, $t0
syscall