# string stuff in mips
.data
str1: .asciiz "Please input string:"
str2: .asciiz "You entered string "
nl: .asciiz "\n"
.text
ori $v0, $0, 4
la $a0, str1
syscall	# should print string "This is a test string" and newline
ori $a0, $0, 0	# save to address 0
ori $a1, $0, 8	# get up to 8 characters
ori $v0, $0, 8  # syscall number 8
syscall         # get string and store
ori $v0, $0, 4
la $a0, nl
syscall # print newline
la $a0, str2
syscall  # will print, "you entered string "
li $a0, 0 # since we saved our input to address 0
syscall # should print our saved string