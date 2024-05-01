#include <unistd.h>
#include <stdio.h>

int
main(void)
{
	while (1) {
		printf("Doing some stuff...\n");
		sleep(3);
	}
	return 0;
}
