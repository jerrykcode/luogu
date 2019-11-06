ifeq ($(OS), Windows_NT)
	# BIT
	BIT = bit.exe
	EXECUTE_BIT = $(BIT)
	CLEAN_CMD = del
	CLEAN_BIT = $(CLEAN_CMD) $(BIT)
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		# BIT
		BIT = bit
		EXECUTE_BIT = ./$(BIT)
		CLEAN_CMD = rm
		CLEAN_BIT = $(CLEAN_CMD) $(BIT)
	endif
endif

TARGETS = $(BIT)

run : $(TARGETS) input output
	echo Execute(BIT):
	$(EXECUTE_BIT) < input > bit_out
	diff bit_out output
	$(CLEAN_CMD) bit_out

clean : $(TARGETS)
	$(CLEAN_BIT)

CC = gcc
CFLAGS = -std=c99

$(BIT) : BIT/P1908.c
	$(CC) -o $@ $<
	
