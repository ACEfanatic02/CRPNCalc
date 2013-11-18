CC=gcc
CFLAGS=-g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG

all:
	mkdir -p bin build
	$(CC) $(CFLAGS) src/RPNCalc.c src/Stack.c src/TokenList.c -o bin/RPNCalc