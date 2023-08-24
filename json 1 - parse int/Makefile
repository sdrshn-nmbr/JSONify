# VARIABLES
#
# ASSIGNMENT INFO, etc.
ASG_NICKNAME = HW10
BASE_NAME = json

# EXECUTABLES
EXECUTABLE = test_$(BASE_NAME)
EXECUTABLE_GCOV = $(EXECUTABLE)_gcov

# SOURCE FILES
SRC_C = $(BASE_NAME).c
TEST_C = $(EXECUTABLE).c
SRC_H = $(BASE_NAME).h clog.h miniunit.h
SUBMIT_FILES = $(TEST_C) $(SRC_C) $(SRC_H) Makefile

# SYSTEM
SHELL = /bin/bash
CC = gcc
CFLAGS = -g -std=c11 -Wall -Wshadow -Wvla -Werror -pedantic
CFLAGS_GCOV = $(CFLAGS) -fprofile-arcs -ftest-coverage


# RULES
#
$(EXECUTABLE): $(SRC_C) $(TEST_C) $(SRC_H)
	$(CC) $(SRC_C) $(TEST_C) -o $(EXECUTABLE) $(CFLAGS)

test: $(EXECUTABLE)
	./$(EXECUTABLE)

valgrind: $(EXECUTABLE)
	valgrind $(EXECUTABLE)

submit: $(SUBMIT_FILES)
	264submit $(ASG_NICKNAME) $(SUBMIT_FILES)

pretest: submit
	264test $(ASG_NICKNAME)

coverage: $(SRC_C) $(TEST_C)
	 $(CC) $(SRC_C) $(TEST_C) -o $(EXECUTABLE_GCOV) $(CFLAGS_GCOV)
	 ./$(EXECUTABLE_GCOV)
	 gcov -f $(SRC_C)

clean: 
	rm -f $(EXECUTABLE) $(EXECUTABLE_GCOV) *.c.gcov *.gcda *.gcno

.PHONY: test valgrind submit pretest coverage clean

# vim: set noexpandtab tabstop=4 filetype=make:
