#!/bin/bash

DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"
cd ${DIR}

tests=(	"cat test.txt"
	"./bin/schedule test.txt"
	"cat test2.txt"
	"./bin/schedule test2.txt"
	)

echo "These are the test cases used for the program:" > demo.txt


for ((i = 0; i < ${#tests[@]}; i++ )); do
	echo "$ ${tests[i]}" &>> demo.txt
	eval "${tests[i]}" &>> demo.txt
	echo "" &>> demo.txt
done

#mv demo.txt ../

exit 0

