#include <stdio.h>
#include <syscall.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
	printf("pid: %i\n", getpid());

	if (argc < 2) {
		printf("need binary\n");
		return 1;
	}

	printf("path: %s\n", argv[1]);

	int file;
	// can't be access from the kernel
	file = syscall(SYS_memfd_secret, 0);
	if (file == -1) {
		printf("failed to memfd, err: %s\n", strerror(errno));
		return 1;
	}

	printf("fd: %i\n", file);
	pause();

	return 0;
}
