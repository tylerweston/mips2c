/*
	mips2c FPU (floating point unit)
	coprocessor 1

	- has 32 registers (numbers $f0...$f31)
*/

// still to do: ALL THIS!

// instructions for dealing with c1
// mtc1 $s0, $f0 move TO cp1 $s0 -> $f0
// mfc1 $s0, $f0 move from cp1 $s0 <- $f0
// lwc1 $f1, 40($s1) load word to memory
// swc1 $f1, 40($s2) load word from memory
// pseudoinstructions:
// l.d $f0, -10($s1) load double precision
// s.d $f0, 12($s2) save double precision
// l.s $f0, -10($s1) load single precision
// s.s $f0, 12($s2) save single precision

// cvt._._
// convert f -> single precision
// d -> double precision, w -> integer