ifeq ($(OS), Windows_NT)
	TARGET = P4552.exe
	EXECUTE = $(TARGET)
	CLEAN = del $(TARGET)
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		TARGET = P4552
		EXECUTE = ./$(TARGET)
		CLEAN = rm $(TARGET)
	endif
endif

run : $(TARGET) input
	$(EXECUTE) < input

clean : $(TARGET)
	$(CLEAN)

CC = gcc
CFLAGS = -std=c99
SRC = P4552.c

$(TARGET) : $(SRC)
	$(CC) -o $@ $< $(CFLAGS)
