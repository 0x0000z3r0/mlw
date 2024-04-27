CFLAGS=-fsanitize=leak,address,undefined -Wall -Wextra

.PHONY:all
all: mlw xor pkr.c
	./xor mlw 9
	mv mlw.x mlw
	ld -r -b binary -z noexecstack mlw -o bin
	$(CC) bin pkr.c -o pkr

.PHONY: pkr
pkr: mlw xor pkr.c
	./xor mlw 9
	mv mlw.x mlw
	ld -r -b binary -z noexecstack mlw -o bin
	$(CC) $(CFLAGS) bin pkr.c -o pkr

.PHONY: xor
xor: xor.c
	$(CC) $(CFLAGS) $^ -o $@

.PHONY: mlw 
mlw: mlw.c mm.c vm.c prc.c
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
	-@rm -rf *.o *.bin *.x mlw xor bin pkr 2>/dev/null || true
