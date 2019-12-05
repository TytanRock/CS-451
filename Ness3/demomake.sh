#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd ${DIR}

tests=(	"cat test.txt"
	"./bin/elevator -p 2 < test.txt"
	"./bin/elevator -p 1 -w 2 -f 2 <<< \"2 1 1 0 1\""
	"./bin/elevator -p 5 <<< \"4 4 2 6 3 4 7 0 2\n3 2 4 10 5 0 4\n2 10 10 0 4\n5 1 1 2 2 3 3 4 4 0 5\n6 4 3 6 4 7 4 2 1 10 10 0 3\""
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

