#define _GNU_SOURCE

#include <stdio.h>
#include <syscall.h>
#include <string.h>
#include <limits.h>
#include <errno.h>
#include <unistd.h>
#include <sys/mman.h>

extern const unsigned char _binary_target_start[];
extern const unsigned char _binary_target_end[];

int
main(int argc, char *argv[])
{
	pid_t pid;
	pid = getpid();
	printf("pid: %i\n", pid);

	if (argc < 2) {
		printf("need binary\n");
		return 1;
	}

	printf("path: %s\n", argv[1]);

	int file;
	file = syscall(SYS_memfd_create, "malware", 0);
	if (file == -1) {
		printf("failed to memfd, err: %s\n", strerror(errno));
		return 1;
	}

	printf("fd: %i\n", file);
	printf("ptr: %p\n", _binary_target_start);
	printf("sig: %02x %02x %02x\n", _binary_target_start[0], _binary_target_start[1], _binary_target_start[2]);

	size_t size;
	size = _binary_target_end - _binary_target_start;

	int res;
	res = ftruncate(file, size);
	if (res == -1) {
		printf("truncate failed, err: %s\n", strerror(errno));
		return 1;
	}

	ssize_t bytes;
	bytes = write(file, _binary_target_start, size);
	if (bytes == -1) {
		printf("write failed, err: %s\n", strerror(errno));
		return 1;
	}
	printf("bytes: %zu\n", bytes);

	char path[PATH_MAX];
	snprintf(path, PATH_MAX, "/proc/%i/fd/%i", pid, file);

	printf("path: %s\n", path);

	char *args[] = { NULL };
	res = fexecve(file, args, args);
	if (res == -1) {
		printf("exec failed, err: %s\n", strerror(errno));
		return 1;
	}

	return 0;
}
