ifeq ($(OS), Windows_NT)
	TARGET = P2574.exe
	EXECUTE = $(TARGET)
	CLEAN_CMD = del
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		TARGET = P2574
		EXECUTE = ./$(TARGET)
		CLEAN_CMD = rm
	endif
endif

CLEAN = $(CLEAN_CMD) $(TARGET)

run : $(TARGET) input output
	$(EXECUTE) < input > tmp_output
	diff output tmp_output
	$(CLEAN_CMD) tmp_output

clean : 
	$(CLEAN)

CC = gcc
CFLAGS = -std=c99
SRC = P2574.c

$(TARGET) : $(SRC)
	$(CC) -o $@ $< $(CFLAGS)
