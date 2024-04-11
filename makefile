CFLAGS=-fsanitize=leak,address,undefined -Wall -Wextra

.PHONY: pkr
pkr: mlw pkr.c
	ld -r -b binary mlw -o bin
	$(CC) $(CFLAGS) bin pkr.c -o pkr

.PHONY: mlw 
mlw: mlw.c elf.c mm.c vm.c
	$(CC) $(CFLAGS) $^ -o $@

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
	-@rm -rf *.o *.bin mlw bin pkr 2>/dev/null || true
