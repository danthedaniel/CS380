CC=gcc
FLAGS=--std=c99 -O0 -ggdb
ARTIFACT=hw2

.DEFAULT_GOAL := build

.PHONY: build clean

build: ${ARTIFACT}

${ARTIFACT}: board.h board.c main.c tree.h tree.c search.h search.c
	  ${CC} ${FLAGS} main.c board.c tree.c search.c -o ${ARTIFACT}

clean:
	  rm ./${ARTIFACT}
