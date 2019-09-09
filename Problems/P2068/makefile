ifeq ($(OS), Windows_NT)	#Windows
	TARGET = P2068.exe
	EXECUTE = $(TARGET)
	CLEAN = del $(TARGET)
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux) #Linux
		TARGET = P2068
		EXECUTE = ./$(TARGET)
		CLEAN = rm $(TARGET)
	endif
endif

run : $(TARGET) input
	$(EXECUTE) < input

clean : 
	$(CLEAN)

CC = gcc
CFLAGS = -std=c99
$(TARGET) : P2068.c
	$(CC) -o $@ $< $(CFLAGS)
