all: clean build

build: main.c power.o
	@gcc -Wall -m32 main.c power.o -o power

power.o: power.asm
	@nasm -g -f elf -o power.o power.asm

clean:
	@rm -f power
	@rm -f power.o
