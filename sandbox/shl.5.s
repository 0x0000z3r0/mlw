section .text

global _start
_start:
	; stack frame setup
	push	rbp
	mov	rbp, rsp
	; allocate 4 bytes
	sub	rsp, 4
	; set "abc\n" in stack
	mov	byte [rsp - 4], 'a'
	mov	byte [rsp - 3], 'b'
	mov	byte [rsp - 2], 'c'
	mov	byte [rsp - 1], 0x0A ; new line in hex
	; call WRITE
	mov	rax, 1		; SYS_write
	mov	rdi, 1		; STDOUT
	lea	rsi, [rsp - 4]  ; buffer
	mov	rdx, 4		; length
	syscall
	; cleanup the setup
	leave
	; call EXIT
	mov	rax, 60		; SYS_exit
	mov	rdi, 0		; return value
	syscall
	; terminate
	ret
