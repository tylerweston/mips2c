![C/C++ CI](https://github.com/tylerweston/mips2c/workflows/C/C++%20CI/badge.svg)
  
<h1>mips2c</h1>
  
mips2c is a mips interpreter written in C. It is currently an unstable work in progress. Support for most of the commands are in there and it can run some simple assembly files, but more complex assembly programs may not function properly. I began writing this without much of an idea of how it would work so the earlier parts of the program (ie, the file parser) is currently a giant spaghetti mess. It is currently being refactored so hopefully most of the horribly designed pieces will be removed sooner rather than later.
  
Usage:  
`./mip2c filename.asm`

There are some example programs in the `examples/` folder. 
```
./mips2c ./examples/guess.asm 
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
  
To view other command line arguments, run with `--help` or `-h`
  
To build:  
```
make
```
  

To run tests after building:  
``` 
make check
```
  
Written by Tyler Weston. 2021. This work is licensed under the terms of the MIT license.
