#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int
main(void)
{
	size_t size;
	size = getpagesize();

	void *mem;
	mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_ANON | MAP_PRIVATE, -1, 0);
	if (mem == MAP_FAILED) {
		printf("Failed to map\n");
		return 1;
	}

	printf("addr: %p, size: %zu\n", mem, size);


	printf("ppid: %i\n", getpid());

	pid_t pid;
	pid = vfork();
	if (pid == -1) {
		printf("Failed to fork\n");
		return 1;
	}

	if (pid == 0) {
		printf("cpid: %i\n", getpid());
		pause();
	} else {
		wait(NULL);
	}

	munmap(mem, size);
	return 0;
}
