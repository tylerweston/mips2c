#/bin/bash
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

testfiles=`ls *.asm`
for testentry in $testfiles
do
	echo -n Testing: ${testentry} "... " 
	filename="${testentry%.*}"
	# To avoid gobbling the last newline, we append an x first
	out=$(../mips2c -l ${testentry} -t; echo x)
	# and then remove it.
	out="${out%?}"
	printf "${out}" > ${filename}.out
	diff ${filename}.out ${filename}.expect && echo -e ${GREEN}Pass || echo -e ${RED}Fail
	echo -n -e ${NC}
done
rm *.out
