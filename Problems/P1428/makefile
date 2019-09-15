ifeq ($(OS), Windows_NT)
	BIT_TARGET = BIT.exe
	SBT_TARGET = SBT.exe
	EXECUTE_CMD =#empty
	CLEAN_CMD = del
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		BIT_TARGET = BIT.out
		SBT_TARGET = SBT.out
		EXECUTE_CMD = ./
		CLEAN_CMD = rm
	endif
endif

TARGETS = $(BIT_TARGET) $(SBT_TARGET)

run : $(TARGETS) input
	$(EXECUTE_CMD)$(BIT_TARGET) < input
	$(EXECUTE_CMD)$(SBT_TARGET) < input

clean : 
	$(CLEAN_CMD) $(BIT_TARGET)
	$(CLEAN_CMD) $(SBT_TARGET)

CC = gcc
CFLAGS = -std=c99
$(BIT_TARGET) : BIT/P1428.c
	$(CC) -o $@ $< $(CFLAGS)

$(SBT_TARGET) : SBT/P1428.c
	$(CC) -o $@ $< $(CFLAGS)
