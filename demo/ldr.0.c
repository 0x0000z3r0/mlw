#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <elf.h>

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("need elf path");
		return 0;
	}

	printf("path: %s\n", argv[1]);

	int file;
	file = open(argv[1], O_RDONLY, S_IRUSR);
	if (file == -1) {
		printf("open failed, err: %s\n", strerror(errno));
		return 0;
	}

	int res; 

	struct stat stat;
	res = fstat(file, &stat);
	if (res == -1) {
		printf("fstat failed, err: %s\n", strerror(errno));
		goto _CLOSE;
	}

	size_t size;
	size = (stat.st_size + (getpagesize() - 1)) & ~(getpagesize() - 1);

	printf("size: %zu (%016lx), required: %i, aligned: %zu (%016lx)\n", stat.st_size, stat.st_size, getpagesize(), size, size);

	void *mem;
	mem = mmap(NULL, size, PROT_READ, MAP_ANON | MAP_PRIVATE, file, 0);
	if (mem == MAP_FAILED) {
		printf("mmap failed, err: %s\n", strerror(errno));
		goto _CLOSE;
	}

_MUNMAP:
	munmap(mem, size);
_CLOSE:
	close(file);
	return 0;
}
