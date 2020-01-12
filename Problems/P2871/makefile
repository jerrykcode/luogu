OJ = P2871

ifeq ($(OS), Windows_NT)
	_1D = $(OJ)_1D.exe
	_2D = $(OJ)_2D.exe
	RUN_CMD = #empty
	CLEAN_CMD = del
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		_1D = $(OJ)_1D
		_2D = $(OJ)_2D
		RUN_CMD = ./
		CLEAN_CMD = rm
	endif
endif

TARGETS := $(_1D) $(_2D)

run : $(TARGETS) input output
	$(foreach var, $(TARGETS), \
		$(RUN_CMD)$(var) < input > tmp_output; \
		diff -b tmp_output output; \
		$(CLEAN_CMD) tmp_output; \
	)

clean : $(TARGETS)
	$(foreach var, $(TARGETS), \
		$(CLEAN_CMD) $(var); \
	)

clean_tmp : tmp_output
	$(CLEAN_CMD) tmp_output

CC = gcc
CFLAGS = -std=c99

$(_1D) : 1D/$(OJ).c
	$(CC) -o $@ $< $(CFLAGS)

$(_2D) : 2D/$(OJ).c
	$(CC) -o $@ $< $(CFLAGS)
