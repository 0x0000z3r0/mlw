#include <sys/syscall.h>
#include <unistd.h>

int
main(void)
{
	// #define SYS_write		1
	// #define STDOUT_FILENO	1
	syscall(1, 1, "abc\n", 4);
	return 0;
}
