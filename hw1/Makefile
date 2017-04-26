CC=gcc
FLAGS=--std=c99 -O0 -ggdb
ARTIFACT=hw1.out

.DEFAULT_GOAL := build

.PHONY: build clean

build: ${ARTIFACT}

${ARTIFACT}: board.h board.c main.c tree.h tree.c
	  ${CC} ${FLAGS} main.c board.c -o ${ARTIFACT}

clean:
	  rm ./${ARTIFACT}
