OJ = P3805
SRC = $(OJ).c
OBJ = $(OJ).o
CC = gcc

windows : $(OJ).exe in out
	$< < in > tmp_out
	-FC tmp_out out
	del tmp_out

linux : $(OJ) in out
	./$< < in > tmp_out
	-diff -b tmp_out out
	rm tmp_out

$(OJ).exe : $(OBJ)
	$(CC) $< -o $@

$(OJ) : $(OBJ)
	$(CC) $< -o $@

$(OBJ) : $(SRC)
	$(CC) -c $<

windows_clean :
	del $(OBJ)
	del $(OJ).exe

linux_clean : 
	rm $(OBJ)
	rm $(OJ)
