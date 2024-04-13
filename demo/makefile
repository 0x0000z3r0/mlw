vdso.0.bin: vdso.0.c
	gcc $^ -o $@

pkr.0.bin: pkr.0.c
	gcc $^ -o $@

pkr.1.bin: pkr.1.c
	gcc $^ -o $@

pkr.2.bin: pkr.2.c
	ld -r -b binary target -o target.o
	gcc target.o pkr.2.c -o $@

pkr.3.bin: pkr.3.c
	ld -r -b binary target -o target.o
	gcc -Og -Wall -Wextra -fsanitize=leak,address,undefined target.o $^ -o $@

pkr.4.bin: pkr.4.c
	ld -r -b binary target -o target.o
	gcc -Og -Wall -Wextra -fsanitize=leak,address,undefined target.o $^ -o $@

pkr.5.bin: pkr.5.c
	gcc -Og -Wall -Wextra -fsanitize=leak,address,undefined $^ -o $@

spl.0.bin: spl.0.c
	gcc $^ -o $@

srv.0.bin: srv.0.c
	gcc -Og -Wall -Wextra -fsanitize=leak,address,undefined $^ -o $@

srv.1.bin: srv.1.c
	gcc -Og -Wall -Wextra -fsanitize=leak,address,undefined $^ -o $@

shl.0.bin: shl.0.c
	gcc $^ -o $@

shl.1.bin: shl.1.c
	gcc $^ -o $@

shl.2.bin: shl.2.c
	gcc $^ -o $@

shl.3.bin: shl.3.c
	gcc $^ -o $@

shl.4.bin: shl.4.c
	gcc $^ -o $@

shl.5.o: shl.5.s
	nasm $^ -O0 -f elf64 -o $@

shl.5.bin: shl.5.o
	ld $^ -o $@

shl.6.bin: shl.6.c
	gcc $^ -o $@

shl.7.bin: shl.7.c
	gcc -Wall $^ -o $@

mem.0.bin: mem.0.c
	gcc -Og -Wall -Wextra -fsanitize=leak,address,undefined $^ -o $@

frk.0.bin: frk.0.c
	gcc $^ -o $@

frk.1.bin: frk.1.c
	gcc $^ -o $@

frk.2.bin: frk.2.c
	gcc $^ -o $@

inj.0.bin: inj.0.c
	gcc -Og -Wall -Wextra -fsanitize=leak,address,undefined $^ -o $@

inj.1.bin: inj.1.c
	gcc -Og -Wall -Wextra -fsanitize=leak,address,undefined $^ -o $@

ldr.0.bin: ldr.0.c
	gcc -Og -Wall -Wextra -fsanitize=leak,address,undefined $^ -o $@

clean:
	-@rm *.bin *.o 2>/dev/null || true
