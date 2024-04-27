#include "def.h"

#include <sys/wait.h>
#include <sys/ptrace.h>

int
prc_lock(pid_t pid)
{
	int res;
	res = ptrace(PTRACE_ATTACH, pid, 0, 0);
	if (res == -1) {
		return PRC_LOCK_ATTACH;
	}

	int sts;
	res = waitpid(pid, &sts, WUNTRACED);
	if (res == -1) {
		return PRC_LOCK_WAIT;
	}

	if (!WIFSTOPPED(sts)) {
		return PRC_LOCK_SIG;
	}

	return PRC_LOCK_OK;
}

int
prc_cont(pid_t pid)
{
	int res;
	res = ptrace(PTRACE_CONT, pid, 0, 0);
	if (res == -1) {
		return PRC_CONT_ERR;
	}

	/*
	int sts;
	res = waitpid(pid, &sts, __WALL | WNOHANG);
	if (res == -1) {
		return PRC_INIT_WAIT;
	}

	if (!WIFCONTINUED(sts)) {
		return PRC_INIT_SIG;
	}
	*/

	return PRC_CONT_OK;
}

int
prc_unlock(pid_t pid)
{
	int res;

	res = ptrace(PTRACE_DETACH, pid, 0, 0);
	if (res == -1) {
		return PRC_UNLOCK_DETACH;
	}

	return PRC_UNLOCK_OK;
}
