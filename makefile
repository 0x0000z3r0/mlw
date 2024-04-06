EXE=malware
CFLAGS=-fsanitize=leak,address,undefined -Wall -Wextra

SRC=$(wildcard *.c)

debug: $(SRC)
	$(CC) $(CFLAGS) $^ -o $(EXE)

shl.bin: shl.o
	ld $^ -o $@

shl.o: shl.s
	nasm $^ -O0 -f elf64 -o $@
	objdump -d $@

srv.bin: srv.o
	ld $^ -o $@

srv.o: srv.s
	nasm $^ -O0 -f elf64 -o $@
	objdump -d $@

clean:
	-@rm -rf $(EXE) *.o *.bin 2>/dev/null || true
