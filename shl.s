%define SYS_write	1
%define SYS_mmap	9

%define PROT_NONE	0x0
%define PROT_READ	0x1
%define PROT_WRITE	0x2
%define PROT_EXEC	0x4

%define MAP_PRIVATE	0x02
%define MAP_ANON	0x20

%define MAP_FAILED	-1

section .text

global _start
_start:
	nop
	nop

	mov	rax, SYS_mmap
	mov	rdi, 0
	mov	rsi, 4096
	mov	rdx, (PROT_READ | PROT_WRITE | PROT_EXEC)
	mov	r10, (MAP_PRIVATE | MAP_ANON)
	mov	r8, -1
	mov	r9, 0
	syscall
	cmp	rax, MAP_FAILED
	je	_start_err

	mov	byte [rax + 0], 'm'
	mov	byte [rax + 1], 'a'
	mov	byte [rax + 2], 'l'
	mov	byte [rax + 3], 'w'
	mov	byte [rax + 4], 'a'
	mov	byte [rax + 5], 'r'
	mov	byte [rax + 6], 'e'

	mov	rsi, rax
	mov	rax, SYS_write
	mov	rdi, 1,
	mov	rdx, 7
	syscall

_start_loop:
	jmp	_start_loop

_start_err:
	ret
