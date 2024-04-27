#include "def.h"

#include <sys/ptrace.h>

int
vm_read(pid_t pid, size_t len, void *buf, const void *addr)
{
	for (size_t i = 0; i < len; ++i) {
		*((char*)buf + i) = ptrace(PTRACE_PEEKDATA, pid, addr, 0);
		++addr;
	}
	return VM_READ_OK;
}

int
vm_write(pid_t pid, size_t len, const void *buf, const void *addr)
{
	int res;

	for (size_t i = 0; i < len; ++i) {
		res = ptrace(PTRACE_POKEDATA, pid, addr, *((char*)buf + i));
		if (res == -1) {
			ptrace(PTRACE_DETACH, pid, 0, 0);
			return VM_WRITE_POKE;
		}

		++addr;
	}
	return VM_WRITE_OK;
}

int
vm_regs_get(pid_t pid, struct user_regs_struct *regs)
{
	int res;

	res = ptrace(PTRACE_GETREGS, pid, NULL, regs);
	if (res == -1) {
		ptrace(PTRACE_DETACH, pid, 0, 0);
		return VM_REGS_GET_REG;
	}

	return VM_REGS_GET_OK;
}

int
vm_regs_set(pid_t pid, struct user_regs_struct *regs)
{
	int res;

	res = ptrace(PTRACE_SETREGS, pid, NULL, regs);
	if (res == -1) {
		ptrace(PTRACE_DETACH, pid, 0, 0);
		return VM_REGS_SET_REG;
	}

	return VM_REGS_SET_OK;
}
