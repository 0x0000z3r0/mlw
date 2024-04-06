#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/prctl.h>
#include <errno.h>

int
main(void)
{
	size_t size;
	size = getpagesize();

	void *mem;
	mem = mmap(NULL, size, PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (mem == MAP_FAILED) {
		printf("Failed to map\n");
		return 1;
	}

	memset(mem, 0xD0, size);
	printf("addr: %p, size: %zu\n", mem, size);

	int res;
	res = mprotect(mem, size, PROT_NONE);
	if (res == -1) {
		printf("Failed to protect the page, err: %s\n", strerror(errno));
		return 1;
	}

	void *mem2;
	mem2 = mmap(NULL, size, PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (mem2 == MAP_FAILED) {
		printf("Failed to map\n");
		return 1;
	}

	memset(mem2, 0xD0, size);
	printf("addr: %p, size: %zu\n", mem2, size);

	res = mprotect(mem2, size, PROT_READ);
	if (res == -1) {
		printf("Failed to protect the page, err: %s\n", strerror(errno));
		return 1;
	}

	/*
	char name[80] = "malware";

	res = prctl(PR_SET_VMA, PR_SET_VMA_ANON_NAME, mem, size, name);
	if (res == -1) {
		printf("Failed to give it a name, res: %i, err: %s\n", res, strerror(errno));
		return 1;
	}
	*/

	pid_t pid;
	pid = fork();
	if (pid == -1) {
		printf("Failed to fork\n");
		return 1;
	}

	if (pid == 0) {
		printf("cpid: %i\n", getpid());
		pause();
	} else {
		printf("ppid: %i\n", getpid());
		wait(NULL);
	}

	munmap(mem, size);
	return 0;
}
