#BIT means binary index tree
#ST means segment tree

ifeq ($(OS), Windows_NT)
	BIT = bit.exe
	EXECUTE_BIT = $(BIT)
	CLEAN_BIT = del $(BIT)

	ST = st.exe
	EXECUTE_ST = $(ST)
	CLEAN_ST = del $(ST)
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		BIT = bit
		EXECUTE_BIT = ./$(BIT)
		CLEAN_BIT = rm $(BIT)

		ST = st
		EXECUTE_ST = ./$(ST)
		CLEAN_ST = rm $(ST)
	endif
endif

run : $(BIT) $(ST) input
	echo execute binary index tree:
	$(EXECUTE_BIT) < input
	echo execute segment tree:
	$(EXECUTE_ST) < input

clean : 
	$(CLEAN_BIT)
	$(CLEAN_ST)

CC = gcc
CFLAGS = -std=c99

BIT_PATH = BIT
$(BIT) : $(BIT_PATH)/P3372.c
	$(CC) -o $@ $< $(CFLAGS)

ST_PATH = SegmentTree
$(ST) : $(ST_PATH)/P3372.c
	$(CC) -o $@ $< $(CFLAGS)
