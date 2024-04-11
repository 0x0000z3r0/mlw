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
	// can be found from kernel, by 'file_inode(file)->i_sb->s_magic == SECRETMEM_MAGIC'
	file = syscall(SYS_memfd_secret, 0);
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

	void *mem;
	mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
	if (mem == MAP_FAILED) {
		printf("map failed, err: %s\n", strerror(errno));
		return 1;
	}

	memcpy(mem, _binary_target_start, size);

	char path[PATH_MAX];
	snprintf(path, PATH_MAX, "/proc/%i/fd/%i", pid, file);

	printf("path: %s\n", path);

	char *args[] = { NULL };
	res = execve(path, args, args);
	if (res == -1) {
		printf("exec failed, err: %s\n", strerror(errno));
		return 1;
	}

	return 0;
}
