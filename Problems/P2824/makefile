ifneq ($(OS), Windows)
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		TARGET = P2824
		EXECUTE_CMD = ./
		CLEAN_CMD = rm
	endif
else
	TARGET = P2824.exe
	EXECUTE_CMD = #empty
	CLEAN_CMD = del
endif

run : $(TARGET) input output
	$(EXECUTE_CMD)$(TARGET) < input > tmp_output
	diff tmp_output output
	$(CLEAN_CMD) tmp_output

clean : 
	$(CLEAN_CMD) $(TARGET)

CC = gcc
CFLAGS = -std=c99
SRC = P2824.c

$(TARGET) : $(SRC)
	$(CC) -o $@ $< $(CFLAGS)
