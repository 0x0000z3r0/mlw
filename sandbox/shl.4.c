int
main(void)
{
	volatile char buf[] = "abc\n";
	unsigned long long len = sizeof (buf) - 1;

	asm volatile(
		"  movq\t	$1, %%rax\n"
		"\tmovq\t	$1, %%rdi\n"
		"\tmovq\t	%0, %%rsi\n"
		"\tmovq\t	%1, %%rdx\n"
		"syscall"
		:
		: "r" (buf), "r" (len)
		: "rax", "rdi"
		);

	return 0;
}
