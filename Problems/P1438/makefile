ifeq ($(OS), Windows_NT)
	TARGET = P1438.exe
	EXECUTE = $(TARGET)
	CLEAN = del $(TARGET)
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		TARGET = P1438
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

$(TARGET) : P1438.c
	$(CC) -o $@ $< $(CFLAGS)
