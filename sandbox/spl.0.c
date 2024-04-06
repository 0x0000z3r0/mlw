#include <stdio.h>
#include <unistd.h>

int
main(void) 
{
	pid_t pid;
	pid = getpid();

	printf("pid: %i\n", pid);

	while (1) {
		printf("%i: doing some work...\n", pid);
		sleep(3);
	}
	return 0;
}
