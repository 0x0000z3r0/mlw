#include <unistd.h>
#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/prctl.h>

int
main(void)
{
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

	return 0;
}
