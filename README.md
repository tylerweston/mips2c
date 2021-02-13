mips2c


Usage:  
`./mip2c -l filename.asm`

There are some example programs in the `examples/` folder. 
```
./mips2c -l ./examples/guess.asm 
MIPS2C
--------------------------
Please guess a number between one and one hundred
:43
43<?
:75
75>?
:57
57=57

```
  
To view other command line arguments, run with `-h`
  
To run tests:  
``` 
chmod +x run_tests.sh  
./run_tests.sh
```
