# CC=gcc
CFLAGS=-g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG

SOURCES=$(wildcard src/*.c)
OBJECTS=$(patsubst %.c, %.o, $(SOURCES))

PROGRAM_SRC=$(wildcard bin/*.c)
PROGRAMS=$(patsubst %.c, %, $(PROGRAM_SRC))

TEST_SRC=$(wildcard tests/*_tests.c)
TESTS=$(patsubst %.c, %, $(TEST_SRC))

all: build $(OBJECTS) $(PROGRAMS) tests

build:
	@mkdir -p build
	@mkdir -p bin

$(PROGRAMS): CFLAGS += $(OBJECTS)

.PHONY: tests
tests: CFLAGS += $(OBJECTS)
tests: $(OBJECTS) $(TESTS)
	sh ./tests/runtests.sh

clean:
	rm -rf build $(OBJECTS) $(TESTS) $(PROGRAMS)
	rm -f tests/tests.log
