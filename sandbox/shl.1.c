#include <unistd.h>

int
main(void)
{
	write(STDOUT_FILENO, "abc\n", 4);
	return 0;
}
