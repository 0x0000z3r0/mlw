mlw: mlw.c
	clang -Wall -Wextra -fsanitize=address,undefined mlw.c -o mlw

shl: shl.o
	ld -o shl shl.o -lSystem -syslibroot `xcrun -sdk macosx --show-sdk-path` -e _start -arch arm64 
	otool -tvVj ./shl.o

shl.o: shl.s
	as -arch arm64 -o shl.o shl.s
