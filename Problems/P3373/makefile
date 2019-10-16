ifeq ($(OS), Windows_NT)
	TARGET = P3373.exe
	EXECUTE = $(TARGET)
	CLEAN = del $(TARGET)
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		TARGET = P3373
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

$(TARGET) : P3373.c
	$(CC) -o $@ $< $(CFLAGS)
