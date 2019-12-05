# branch count
ori $t0, $zero, 0
ori $t1, $zero, 10
ori $t2, $zero, 1
ori $t3, $zero, 5
ori $v0, $zero, 1    # print integer syscall
loop1:
add $t0, $t0, $t2
add $a0, $t0, $zero
syscall
beq $t0, $t3, loop2
bne $t0, $t1, loop1
loop2: