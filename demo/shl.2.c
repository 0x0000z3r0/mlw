#include <sys/syscall.h>
#include <unistd.h>

int
main(void)
{
	// #define SYS_write		1
	// #define STDOUT_FILENO	1
	syscall(SYS_write, STDOUT_FILENO, "abc\n", 4);
	return 0;
}
