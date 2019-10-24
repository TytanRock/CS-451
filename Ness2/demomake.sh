#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd ${DIR}/bin

tests=(	"./schedule" 
	)

echo "These are the test cases used for the program:" >> demo.txt


for ((i = 0; i < ${#tests[@]}; i++ )); do
	echo "$ ${tests[i]}" &>> demo.txt
	eval "${tests[i]}" &>> demo.txt
	echo "" &>> demo.txt
done

mv demo.txt ../

exit 0
