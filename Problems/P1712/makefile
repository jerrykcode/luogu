ifeq ($(OS), Windows_NT)
	TARGET = P1712.exe
	EXECUTE_CMD = #empty
	CLEAN_CMD = del
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		TARGET = P1712
		EXECUTE_CMD = ./
		CLEAN_CMD = rm
	endif
endif


run : $(TARGET) input output
	$(EXECUTE_CMD)$(TARGET) < input > tmp_output
	diff tmp_output output
	$(CLEAN_CMD) tmp_output

clean : 
	$(CLEAN_CMD) $(TARGET)

CC = gcc
CFLAGS = -std=c99
SRC = P1712.c

$(TARGET) : $(SRC)
	$(CC) -o $@ $< $(CFLAGS)
