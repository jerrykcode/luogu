ifeq ($(OS), Windows_NT)
	TARGET = P1168.exe
	EXECUTE = $(TARGET)
	CLEAN = del $(TARGET)
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		TARGET = P1168
		EXECUTE = ./$(TARGET)
		CLEAN = rm $(TARGET)
	endif
endif

run : $(TARGET) input
	$(EXECUTE) < input

clean : 
	$(CLEAN)

CC = g++
$(TARGET) : P1168.cc
	$(CC) -o $@ $<
