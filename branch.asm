# branch count
loop2:
ori $t0, $zero, 0
ori $t1, $zero, 10
ori $t2, $zero, 1
loop1:
add $t0, $t0, $t2
bne $t0, $t1, loop1
