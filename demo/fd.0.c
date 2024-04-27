#include <stdio.h>
#include <unistd.h>

int
main(void)
{
	write(STDOUT_FILENO, "1\n", 2);
	dup2(STDERR_FILENO, STDOUT_FILENO);
	close(STDERR_FILENO);
	write(STDOUT_FILENO, "2\n", 2);

	pause();
	return 0;
}
