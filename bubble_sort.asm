# from: https://raw.githubusercontent.com/atorres1929/MIPS-BubbleSort/master/bubble_sort.asm
#
      .data
#nums: .word   0 : 12        # "array" of 12 words to contain 

values
size: .word  12             # size of "array" 
      .text
      la $s0, nums
      la $s5, size        # load address of size variable
      lw $s5, 0($s5)	    # load value of size
      
      # Populate with twelve values
      addi $t1, $zero, 55
      sw $t1, 0($s0)
      addi $t1, $zero, 88
      sw $t1, 4($s0)
      addi $t1, $zero, 0
      sw $t1, 8($s0)
      addi $t1, $zero, 22
      sw $t1, 12($s0)
      addi $t1, $zero, 77
      sw $t1, 16($s0)
      addi $t1, $zero, 44
      sw $t1, 20($s0)
      addi $t1, $zero, 99
      sw $t1, 24($s0)
      addi $t1, $zero, 33
      sw $t1, 28($s0)
      addi $t1, $zero, 110
      sw $t1, 32($s0)
      addi $t1, $zero, 66
      sw $t1, 36($s0)
      addi $t1, $zero, 121
      sw $t1, 40($s0)
      addi $t1, $zero, 11
      sw $t1, 44($s0)

      ##################################################################
      # AT THIS POINT: $s0 is the address of the start of the array
      #                $s5 is the size (= 12)
      #################################################################
      
      ##################################################################
      # PUT CODE HERE FOR CALL
      ##############################
      # SAVE $a registers and $ra and $s registers
      ##############################
      addi $sp, $sp, -20
      sw $a0, 0($sp)
      sw $a1, 4($sp)
      sw $ra, 8($sp)
      sw $s0, 12($sp)
      sw $s5, 16($sp)
      ##############################
      # Change $a registers
      ##############################
      add $a0, $zero, $s0
      add $a1, $zero, $s5
      ##############################
      # jal
      ##############################
      jal bubblesort
      ##############################
      # RELOAD $a registers and $ra and $s registers
      ##############################
      lw $a0, 0($sp)
      lw $a1, 4($sp)
      lw $ra, 8($sp)
      lw $s0, 12($sp)
      lw $s5, 16($sp)
      addi $sp, $sp, 20
      ##################################################################

                         
      ##################################################################
      # DO NOT MODIFY
      la   $a0, nums        # first argument for print (array)
      add  $a1, $s5, $zero      # second argument for print (size)
      jal  print            # call print routine. 
      li   $v0, 10          # system call for exit
      syscall               # we are out of here.
      ##################################################################


########################################################################
# PUT CODE HERE FOR FUNCTION
bubblesort: # int[] arr(a0), int n(a1) 
      ##################################################################
      # IF STATEMENT
      li $t7, 1
      beq $a1, $t7, endsort 	# if n == 1 then goto over
      ############################
      # RETURN FROM FUNCTION
      ############################
      ##################################################################
      
      ##################################################################
      # FOR LOOP
      ##################################################################
      	add $s1, $zero, $zero 	# j = 0
        add $t0, $zero, $zero 	# target
       
        addi $s2, $a1, -1  	# n - 1
      while:			# while
      	slt $t0, $s1, $s2	# j < n-1
            li $t7, 1
      	bne $t0, $t7, done	# if j < n - 1 THEN continue, else GOTO done
      	
      	# Need arr[j] -> index = j * 4 so IF j = 4 THEN index = 16 (remember word size is 4)
      	add $t1, $s1, $s1	# t1 = j + j
      	add $t1, $t1, $t1	# t1 = j + j (j is now mulitplied by 4)
      	add $t1, $a0, $t1
      		# t1 = index 0 + index j
      	lw $t2, 0($t1)		# load word at t1 which is arr[j]
      	
      	# Need arr[j+1] -> index = (j+1) * 4 so IF j = 4 THEN index = 20 (4+1=5 -> 5*4=20)
      	addi $t3, $s1, 1	# t3 = j + 1
      	add $t3, $t3, $t3	# t3 = (j+1) + (j+1)
      	add $t3, $t3, $t3	# t3 = (j+1) + (j+1) (j+1 is now multiplied by 4)
      	add $t3, $s0, $t3	# t3 = index 0 + index j + 1
      	lw $t4, 0($t3)		# load word at t3 which is arr[j+1]
      	
      	add $t0, $zero, $zero   # target
      	slt $t0, $t4, $t2	# arr[j+1] < arr[j]
      	bne $t0, 1, endif	# if slt NOT true, end of if
      	sw $t2, 0($t3)		# swap j and j+1 -> j goes into j + 1 (address location)
      	sw $t4, 0($t1)		# swap j+1 and j -> j + 1 goes into j (address location)
      endif:
      ##################################################################
      # RECURSIVE CALL
      	addi $s1, $s1, 1	# increment j
      	j while			# jump to the top of the loop
      done:
      ######################################
      # SAVE $a registers, $ra
      ######################################
      	addi $sp, $sp, -12	
	sw $a0 0($sp)				
      	sw $a1 4($sp)				
      	sw $ra 8($sp)				
      ######################################
      # CHANGE $a registers
      ######################################
      	addi $a1, $a1, -1		# decrement the size of the array to search from 12, 11, 10 etc. 	
      ######################################
      # jal
      ######################################
        jal bubblesort
      ######################################
      # RELOAD $a registers, $ra
      ######################################
        lw $a0, 0($sp)			
      	lw $a1, 4($sp)			
      	lw $ra, 8($sp)			
      	addi $sp, $sp, 12		
      ################################################################## 
      endsort:
      	jr $ra
      ##################################################################
      # RETURN FROM FUNCTION
      ##################################################################
      
########################################################################


########################################################################
#########  routine to print the numbers on one line. 
#########  don't touch anything below this line!!!!

      .data
space:.asciiz " "
head: .asciiz "Sorted array:\n"
      .text
print:add  $s0, $zero, $a0  # starting address of array
      add  $t1, $zero, $a1  # initialize loop counter to array size
      la   $a0, head        # load address of print heading
      li   $v0, 4           # specify Print String service
      syscall               # print heading
out:  lw   $a0, 0($s0)      # load number for syscall
      li   $v0, 1           # specify Print Integer service
      syscall               # print number
      la   $a0, space       # load address of spacer for syscall
      li   $v0, 4           # specify Print String service
      syscall               # output string
      addi $s0, $s0, 4      # increment address
      addi $t1, $t1, -1     # decrement loop counter
      bgtz $t1, out         # repeat if not finished
      jr   $ra              # return
########################################################################
.data
nums: .word  0
	