int
main(void)
{
	register long sys  asm ("rax") = 1;
	register long std  asm ("rdi") = 1;
	register char *buf asm ("rsi") = "abc\n";
	register long len  asm ("rdx") = 4;

	asm volatile("syscall" : : "r" (sys), "r" (std), "r" (buf), "r" (len) : "memory", "rcx", "r11", "cc");

	return 0;
}
