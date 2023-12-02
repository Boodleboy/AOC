default: bin
	cat input.txt | ./bin

bin: code.c
	gcc -o bin code.c

test: bin
	cat test.txt | ./bin
