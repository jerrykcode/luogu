OJ = P1616

ifeq ($(OJ), Windows_NT)
	TARGET = $(OJ).exe
	RUN = $(TARGET)
	CLEAN_CMD = del
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		TARGET = $(OJ)
		RUN = ./$(TARGET)
		CLEAN_CMD = rm
	endif
endif

run : $(TARGET) input output
	$(RUN) < input > tmp_output
	diff -b output tmp_output
	$(CLEAN_CMD) tmp_output

clean : $(TARGET)
	$(CLEAN_CMD) $<

clean_tmp_output : tmp_output
	$(CLEAN_CMD) tmp_output

CC = gcc
CFLAGS = -std=c99
SRC = $(OJ).c

$(TARGET) : $(SRC)
	$(CC) -o $@ $< $(CFLAGS)
