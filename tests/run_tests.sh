#/bin/bash
testfiles=`ls *.asm`
for testentry in $testfiles
do
	echo Testing: ${testentry}
	filename="${testentry%.*}"
	out=`../mips2c -l ${testentry} -t`
	#../mips2c -l ${testentry} -t > ${filename}.out
	# TODO: Is this handling newlines properly?
	echo ${out} > ${filename}.out
	diff ${filename}.out ${filename}.expect && echo Pass || echo Fail
done
rm *.out
