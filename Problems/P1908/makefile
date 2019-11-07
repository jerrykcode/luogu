ifeq ($(OS), Windows_NT)
	# clean cmd
	CLEAN_CMD = del
	# execute cmd
	EXECUTE_CMD = #empty
	# BIT
	BIT = bit.exe
	EXECUTE_BIT = $(BIT)
	
	# SBT
	SBT = sbt.exe
	EXECUTE_SBT = $(SBT)
	
	# echo
	ECHO = #empty
else
	UNAME_S := $(shell uname -s)
	ifeq ($(UNAME_S), Linux)
		# clean cmd
		CLEAN_CMD = rm
		# execute cmd
		EXECUTE_CMD = ./
		# BIT
		BIT = bit
		EXECUTE_BIT = $(EXECUTE_CMD)$(BIT)
		# SBT
		SBT = sbt
		EXECUTE_SBT = $(EXECUTE_CMD)$(SBT)
		# echo
		ECHO = " #"
	endif
endif

# clean
CLEAN_BIT = $(CLEAN_CMD) $(BIT)
CLEAN_SBT = $(CLEAN_CMD) $(SBT)

TARGETS = $(BIT) $(SBT)

run : $(TARGETS) input output
	echo $(ECHO)Execute(BIT):$(ECHO)
	$(EXECUTE_BIT) < input > bit_out
	diff bit_out output
	$(CLEAN_CMD) bit_out
	echo $(ECHO)Execute(SBT):$(ECHO)
	$(EXECUTE_SBT) < input > sbt_out
	diff sbt_out output
	$(CLEAN_CMD) sbt_out

clean : $(TARGETS) # clean all the targets
	$(CLEAN_BIT)
	$(CLEAN_SBT)

clean_bit : $(BIT) # clean bit executable file
	$(CLEAN_BIT)

clean_sbt : $(SBT) # clean sbt executable file
	$(CLEAN_SBT)

CC = gcc
CFLAGS = -std=c99

$(BIT) : BIT/P1908.c
	$(CC) -o $@ $< $(CFLAGS)

$(SBT) : SBT/P1908_sbt.c
	$(CC) -o $@ $< $(CFLAGS)
