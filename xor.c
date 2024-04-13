#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <limits.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
	if (argc < 3) {
		printf("need <file> <key>\n");
		return 1;
	}

	char *in;
	in = argv[1];
	printf("file: %s\n", in);

	unsigned char key;
	key = atoi(argv[2]);
	printf("key: 0x%02x\n", key);

	int file;
	file = open(in, O_RDONLY, S_IRUSR);
	if (file == -1) {
		printf("open failed, err: %s\n", strerror(errno));
		return 1;
	}

	int res;

	struct stat stat;
	res = fstat(file, &stat);
	if (res == -1) {
		printf("stat failed, err: %s\n", strerror(errno));
		return 1;
	}

	unsigned char *mem;
	mem = mmap(NULL, stat.st_size, PROT_READ, MAP_PRIVATE, file, 0); 
	if (mem == MAP_FAILED) {
		printf("mmap failed, err: %s\n", strerror(errno));
		return 1;
	}

	close(file);

	char path[PATH_MAX];
	snprintf(path, sizeof (path), "%s.x", argv[1]);

	printf("out: %s\n", path);

	file = open(path, O_CREAT | O_RDWR | O_TRUNC, 0777);
	if (file == -1) {
		printf("open failed, err: %s\n", strerror(errno));
		return 1;
	}

	res = ftruncate(file, stat.st_size);
	if (res == -1) {
		printf("ftruncate failed, err: %s\n", strerror(errno));
		return 1;
	}

	unsigned char *out;
	out = mmap(NULL, stat.st_size, PROT_WRITE, MAP_SHARED, file, 0);
	if (out == MAP_FAILED) {
		printf("mmap failed, err: %s\n", strerror(errno));
		return 1;
	}

	for (off_t i = 0; i < stat.st_size; ++i) {
		out[i] = mem[i] ^ key;
	}
	
	res = msync(out, stat.st_size, MS_SYNC);
	if (res == -1) {
		printf("msync failed, err: %s\n", strerror(errno));
		return 1;
	}

	munmap(out, stat.st_size);
	close(file);
	munmap(mem, stat.st_size);
	return 0;
}
