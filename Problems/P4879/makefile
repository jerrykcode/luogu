ifeq ($(OS), Windows_NT)
	TARGET = P4879.exe
	EXECUTE = $(TARGET)
	CLEAN = del $(TARGET)
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		TARGET = P4879
		EXECUTE = ./$(TARGET)
		CLEAN = rm $(TARGET)
	endif
endif

CC = gcc
CFLAGS = -std=c99

run : $(TARGET) input
	$(EXECUTE) < input

clean : 
	$(CLEAN)

$(TARGET) : P4879.c
	$(CC) -o $(TARGET) P4879.c $(CFLAGS)
