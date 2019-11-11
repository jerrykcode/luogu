ifeq ($(OS), Windows_NT)
	TARGET = P1558.exe
	EXECUTE = $(TARGET)
	CLEAN_CMD = del	
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		TARGET = P1558
		EXECUTE = ./$(TARGET)
		CLEAN_CMD = rm
	endif
endif

run : $(TARGET) input output
	$(EXECUTE) < input > tmp_output
	diff output tmp_output
	$(CLEAN_CMD) tmp_output

clean : $(TARGET)
	$(CLEAN_CMD) $(TARGET)

CC = gcc
CFLAGS = -std=c99
SRC = P1558.c

$(TARGET) : $(SRC)
	$(CC) -o $@ $< $(CFLAGS)

