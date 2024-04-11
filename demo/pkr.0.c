#include <stdio.h>
#define _GNU_SOURCE
#include <sys/mman.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

extern int 
memfd_create(const char *name, unsigned int flags);

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
	file = memfd_create("test_memfd_file", 0);
	if (file == -1) {
		printf("failed to memfd, err: %s\n", strerror(errno));
		return 1;
	}

	printf("fd: %i\n", file);
	pause();

	return 0;
}
