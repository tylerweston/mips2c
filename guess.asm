#guessing game
.text
start:
ori $t0, $zero, 57    	# $t0 contains the number to guess!
guessagain:
ori $v0, $zero, 5    	# set $v0 to 5, this will be replaced with the int value we get back
syscall              	# get our integer guess
beq $t0, $v0, winner   	# if we guess correct, we win
sub $s1, $t0, $v0
blez $s1, toohigh   	# if we get here, our guess was too low

toolow:
add $a0, $zero, $v0		# print your guess (stored in $v0)
ori $v0, $zero, 1		#print int
syscall
ori $v0, $zero, 11		# print char syscall
ori $a0, $zero, 60		# char 60
syscall				
ori $v0, $zero, 11		# print char syscall
ori $a0, $zero, 63		# question mark
syscall
ori $v0, $zero, 11		# print char syscall
ori $a0, $zero, 10		# new line
syscall
beq $zero, $zero, guessagain

toohigh:
add $a0, $zero, $v0		# print your guess (stored in $v0)
ori $v0, $zero, 1		#print int
syscall
ori $v0, $zero, 11		# print char syscall
ori $a0, $zero, 62		# greater than sign
syscall
ori $v0, $zero, 11		# print char syscall
ori $a0, $zero, 63		# question mark
syscall
ori $v0, $zero, 11		# print char syscall
ori $a0, $zero, 10		# new line
syscall
beq $zero, $zero, guessagain

winner:
add $a0, $zero, $v0		# print your guess (stored in $v0)
add $t0, $zero, $v0
ori $v0, $zero, 1		# print int
syscall
ori $v0, $zero, 11		# print char syscall
ori $a0, $zero, 61		# equals sign
syscall					# do a syscall
add $a0, $zero, $t0		# print your guess (stored in $v0)
ori $v0, $zero, 1		# print int
syscall
ori $v0, $zero, 11		# print char syscall
ori $a0, $zero, 10		# new line
syscall
