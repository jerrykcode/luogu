OJ = P5146

ifeq ($(OS), Windows_NT)
	TARGET_0 = $(OJ)_0.exe
	TARGET_1 = $(OJ)_1.exe
	TARGET_2 = $(OJ)_2.exe
	RUN_CMD = #empty
	CLEAN_CMD = del
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		TARGET_0 = $(OJ)_0
		TARGET_1 = $(OJ)_1
		TARGET_2 = $(OJ)_2
		RUN_CMD = ./
		CLEAN_CMD = rm
	endif
endif

TARGETS := $(TARGET_0) $(TARGET_1) $(TARGET_2)

DIFF = diff -b

run : $(TARGETS) input output
	-$(foreach var, $(TARGETS), \
		$(RUN_CMD)$(var) < input > tmp_output; \
		$(DIFF) tmp_output output; \
		$(CLEAN_CMD) tmp_output; \
	)

clean : $(TARGETS)
	$(foreach var, $(TARGETS), \
		$(CLEAN_CMD) $(var); \
	)

CC = gcc

$(TARGET_0) : $(OJ)_0.c
	$(CC) -o $@ $<

$(TARGET_1) : $(OJ)_1.c
	$(CC) -o $@ $<

$(TARGET_2) : $(OJ)_2.c
	$(CC) -o $@ $<
