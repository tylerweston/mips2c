#guessing game
.data
intro: .asciiz "Please guess a number between one and one hundred\n"
.text
start:
li $t0, 57              # $t0 contains the number to guess!
ori $v0, $0, 4
la $a0, intro
syscall	
guessagain:
li $v0, 11
li $a0, 58
syscall
li $v0, 5               # read int syscall
syscall              	# get our integer guess
beq $t0, $v0, winner   	# if we guess correct, we win
sub $s1, $t0, $v0
blez $s1, toohigh   	# if we get here, our guess was too low

toolow:
add $a0, $zero, $v0		# print your guess (stored in $v0)
li $v0, 1	        	# print int
syscall
li $v0, 11        		# print char syscall
li $a0, 60        		# char 60
syscall				
li $v0, 11        		# print char syscall
li $a0, 63        		# question mark
syscall
li $v0, 11        		# print char syscall
li $a0, 10        		# new line
syscall
beq $zero, $zero, guessagain

toohigh:
add $a0, $zero, $v0		# print your guess (stored in $v0)
li $v0, 1		        #print int
syscall
li $v0, 11	        	# print char syscall
li $a0, 62	        	# greater than sign
syscall
li $v0, 11	        	# print char syscall
li $a0, 63	        	# question mark
syscall
li $v0, 11	        	# print char syscall
li $a0, 10	        	# new line
syscall
beq $zero, $zero, guessagain

winner:
add $a0, $zero, $v0		# print your guess (stored in $v0)
add $t0, $zero, $v0
li $v0, 1		        # print int
syscall
li $v0, 11	        	# print char syscall
li $a0, 61	        	# equals sign
syscall					# do a syscall
add $a0, $zero, $t0		# print your guess (stored in $v0)
li $v0, 1		        # print int
syscall
li $v0, 11	        	# print char syscall
li $a0, 10	        	# new line
syscall
