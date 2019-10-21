#!/bin/bash

tests=(	"./bin/5ps" 
	"./bin/5ps -h"
	"./bin/5ps -p 0" 
	"./bin/5ps -p" 
	"./bin/5ps -p 1" 
	"./bin/5ps -hp 1" 
	"./bin/5ps -ctvs"
	"./bin/5ps -ctvsp 1" 
	"./bin/5ps -ptcvs 1" 
	"./bin/5ps -ptchvs 1"
	"./bin/5ps -tcvsp 1" 
	"./bin/5ps -t -c 192" 
	"./bin/5ps -p 1 - -v -s"
	)

echo "This is the current state of pid 1:" > demo.txt
ps -lyp 1 &>> demo.txt
echo >> demo.txt

echo "-------------------------------------------------------" &>> demo.txt

echo "These are the test cases used for the program:" >> demo.txt


for ((i = 0; i < ${#tests[@]}; i++ )); do
	echo "$ ${tests[i]}" &>> demo.txt
	eval "${tests[i]}" &>> demo.txt
	echo "" &>> demo.txt
done

exit 0

