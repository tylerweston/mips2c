.data
comma: .asciiz ", "
.text
# branch count
ori $t0, $zero, 0	# initial value of loop
ori $t1, $zero, 10	# end condition
ori $t2, $zero, 1   # number to skip each time
loop1: 
beq $t0, $zero, skipprint
ori $v0, $0, 4
la $a0, comma
syscall
skipprint:
add $t0, $t0, $t2
add $a0, $t0, $zero
ori $v0, $zero, 1    # print integer syscall
syscall
bne $t0, $t1, loop1
