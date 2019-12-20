#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd ${DIR}

tests=(	"cat test.txt"
	"./bin/allocator 1000000 < test.txt"
	)

echo "These are the test cases used for the program:" > demo.txt

echo "Performing tests"
for ((i = 0; i < ${#tests[@]}; i++ )); do
	echo "Test ${tests[i]} running"
	echo "$ ${tests[i]}" &>> demo.txt
	eval "${tests[i]}" &>> demo.txt
	echo "" &>> demo.txt
	echo "Test ${tests[i]} done"
done

#mv demo.txt ../

exit 0

