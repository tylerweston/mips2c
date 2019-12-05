# comment at start of line
# will this leave multiple blank lines?
ori $v0, $0, 69       # this is a comment
label1:
ori $v1, $zero, 351		# i guess another comment?
ori $a0, $zero, 4
label2:
add $a0, $v0, $v1  	  # also a comment
syscall