#define _GNU_SOURCE

#include <stdio.h>
#include <syscall.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

extern const unsigned char _binary_target_start[];
extern const unsigned char _binary_target_end[];

int
main(int argc, char *argv[])
{
	printf("pid: %i\n", getpid());

	if (argc < 2) {
		printf("need binary\n");
		return 1;
	}

	printf("path: %s\n", argv[1]);

	int mem;
	mem = syscall(SYS_memfd_secret, 0);
	if (mem == -1) {
		printf("failed to memfd, err: %s\n", strerror(errno));
		return 1;
	}

	printf("fd: %i\n", mem);
	printf("ptr: %p\n", _binary_target_start);
	printf("sig: %02x %02x %02x\n", _binary_target_start[0], _binary_target_start[1], _binary_target_start[2]);

	return 0;
}
