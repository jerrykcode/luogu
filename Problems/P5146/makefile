OJ = P5146

ifeq ($(OS), Windows_NT)
	TARGET_0 = $(OJ)_0.exe
	TARGET_1 = $(OJ)_1.exe
	RUN_CMD = #empty
	CLEAN_CMD = del
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		TARGET_0 = $(OJ)_0
		TARGET_1 = $(OJ)_1
		RUN_CMD = ./
		CLEAN_CMD = rm
	endif
endif

TARGETS := $(TARGET_0) $(TARGET_1)

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

clean_tmp_output : tmp_output
	$(CLEAN_CMD) $<

CC = gcc

$(TARGET_0) : $(OJ)_0.c
	$(CC) -o $@ $<

$(TARGET_1) : $(OJ)_1.c
	$(CC) -o $@ $<
