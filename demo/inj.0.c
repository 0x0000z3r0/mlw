#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <sys/types.h>

extern ssize_t 
process_vm_readv(pid_t pid,
		const struct iovec *local_iov,
                unsigned long liovcnt,
                const struct iovec *remote_iov,
                unsigned long riovcnt,
                unsigned long flags);

int 
main(int argc, char *argv[]) 
{
	if (argc < 3) {
		printf("Supplt pid and addr\n");
		return 1;
	}

	pid_t pid;
	pid = atoi(argv[1]);

	printf("pid: %i\n", pid);

	long addr;
	addr = strtol(argv[2], NULL, 16);
	printf("addr: 0x%016lx\n", addr);

	int res;
	res = ptrace(PTRACE_ATTACH, pid, 0, 0);
	if (res == -1) {
		printf("Failed to attach, err: %s\n", strerror(errno));
		return 1;
	}

	waitpid(pid, NULL, WUNTRACED);

	long val = ptrace(PTRACE_PEEKDATA, pid, addr, 0);
	printf("ptrace, val: 0x%016lx, err: %s\n", val, strerror(errno));

	struct iovec local[1];
	local[0].iov_base = &val;
	local[0].iov_len  = sizeof (val);

	struct iovec remote[1];
	remote[0].iov_base = (void*)addr;
	remote[0].iov_len  = local[0].iov_len;

	ssize_t bytes;
	bytes = process_vm_readv(pid, local, 1, remote, 1, 0);
	if (bytes == -1) {
		printf("VM Read failed, err: %s\n", strerror(errno));
		return 1;
	}

	printf("vm_read, val: 0x%016lx, err: %s\n", val, strerror(errno));

	return 0;
}
