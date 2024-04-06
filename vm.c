#include "def.h"

#include <sys/wait.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/ptrace.h>

extern ssize_t 
process_vm_readv(pid_t pid,
		const struct iovec *local_iov,
                unsigned long liovcnt,
                const struct iovec *remote_iov,
                unsigned long riovcnt,
                unsigned long flags);

extern ssize_t 
process_vm_writev(pid_t pid,
		const struct iovec *local_iov,
                unsigned long liovcnt,
                const struct iovec *remote_iov,
                unsigned long riovcnt,
                unsigned long flags);

int
vm_read(pid_t pid, size_t len, void *buf, const void *addr, char prot)
{
	if (prot & (1 << MM_PROT_READ)) {
		struct iovec local[1];
		local[0].iov_base = buf;
		local[0].iov_len  = len;

		struct iovec remote[1];
		remote[0].iov_base = (void*)addr;
		remote[0].iov_len  = local[0].iov_len;

		ssize_t bytes;
		bytes = process_vm_readv(pid, local, 1, remote, 1, 0);
		if (bytes == -1) {
			return VM_READ_ERR;
		}

		return VM_READ_OK;
	}

	int res;
	res = ptrace(PTRACE_ATTACH, pid, 0, 0);
	if (res == -1) {
		return VM_READ_ATTACH;
	}

	res = waitpid(pid, NULL, WUNTRACED);
	if (res == -1) {
		return VM_READ_WAIT;
	}

	for (size_t i = 0; i < len; ++i) {
		*((char*)buf + i) = ptrace(PTRACE_PEEKDATA, pid, addr, 0);
		++addr;
	}

	res = ptrace(PTRACE_DETACH, pid, 0, 0);
	if (res == -1) {
		return VM_READ_DETACH;
	}

	return VM_READ_PTRACE;
}

int
vm_write(pid_t pid, size_t len, const void *buf, const void *addr, char prot)
{
	if (prot & (1 << MM_PROT_WRITE)) {
		struct iovec local[1];
		local[0].iov_base = (void*)buf;
		local[0].iov_len  = len;

		struct iovec remote[1];
		remote[0].iov_base = (void*)addr;
		remote[0].iov_len  = local[0].iov_len;

		ssize_t bytes;
		bytes = process_vm_writev(pid, local, 1, remote, 1, 0);
		if (bytes == -1) {
			return VM_WRITE_ERR;
		}

		return VM_WRITE_OK;
	}

	int res;
	res = ptrace(PTRACE_ATTACH, pid, 0, 0);
	if (res == -1) {
		return VM_WRITE_ATTACH;
	}

	res = waitpid(pid, NULL, WUNTRACED);
	if (res == -1) {
		return VM_WRITE_WAIT;
	}

	for (size_t i = 0; i < len; ++i) {
		res = ptrace(PTRACE_POKEDATA, pid, addr, *((char*)buf + i));
		if (res == -1) {
			ptrace(PTRACE_DETACH, pid, 0, 0);
			return VM_WRITE_POKE;
		}

		++addr;
	}

	res = ptrace(PTRACE_DETACH, pid, 0, 0);
	if (res == -1) {
		return VM_WRITE_DETACH;
	}

	return VM_WRITE_PTRACE;
}

int
vm_regs_get(pid_t pid, struct user_regs_struct *regs)
{
	int res;
	res = ptrace(PTRACE_ATTACH, pid, 0, 0);
	if (res == -1) {
		return VM_REGS_GET_ATTACH;
	}

	res = waitpid(pid, NULL, WUNTRACED);
	if (res == -1) {
		return VM_REGS_GET_WAIT;
	}

	res = ptrace(PTRACE_GETREGS, pid, NULL, regs);
	if (res == -1) {
		ptrace(PTRACE_DETACH, pid, 0, 0);
		return VM_REGS_GET_REGS;
	}

	res = ptrace(PTRACE_DETACH, pid, 0, 0);
	if (res == -1) {
		return VM_REGS_GET_DETACH;
	}

	return VM_REGS_GET_OK;
}

int
vm_regs_set(pid_t pid, struct user_regs_struct *regs)
{
	int res;
	res = ptrace(PTRACE_ATTACH, pid, 0, 0);
	if (res == -1) {
		return VM_REGS_SET_ATTACH;
	}

	res = waitpid(pid, NULL, WUNTRACED);
	if (res == -1) {
		return VM_REGS_SET_WAIT;
	}

	res = ptrace(PTRACE_SETREGS, pid, NULL, regs);
	if (res == -1) {
		ptrace(PTRACE_DETACH, pid, 0, 0);
		return VM_REGS_SET_REGS;
	}

	res = ptrace(PTRACE_DETACH, pid, 0, 0);
	if (res == -1) {
		return VM_REGS_SET_DETACH;
	}

	return VM_REGS_SET_OK;
}
