#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/ptrace.h>
#include <string.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <sys/user.h>

/*
  -2:   90			nop 
  -1:   90			nop

   0:   55                      push   %rbp
   1:   48 89 e5                mov    %rsp,%rbp
   4:   48 81 ec 04 00 00 00    sub    $0x4,%rsp
   b:   c6 44 24 fc 61          movb   $0x61,-0x4(%rsp)
  10:   c6 44 24 fd 62          movb   $0x62,-0x3(%rsp)
  15:   c6 44 24 fe 63          movb   $0x63,-0x2(%rsp)
  1a:   c6 44 24 ff 0a          movb   $0xa,-0x1(%rsp)
  1f:   48 b8 01 00 00 00 00    movabs $0x1,%rax
  26:   00 00 00 
  29:   48 bf 01 00 00 00 00    movabs $0x1,%rdi
  30:   00 00 00 
  33:   48 8d 74 24 fc          lea    -0x4(%rsp),%rsi
  38:   48 ba 04 00 00 00 00    movabs $0x4,%rdx
  3f:   00 00 00 
  42:   0f 05                   syscall
  44:   c9                      leave
;  45:   c3                      ret
*/

static const unsigned char code[] = {
	0x90,
	0x90,

	0x55,
	0x48, 0x89, 0xe5,
	0x48, 0x81, 0xec, 0x04, 0x00, 0x00, 0x00,
	0xc6, 0x44, 0x24, 0xfc, 0x61,
	0xc6, 0x44, 0x24, 0xfd, 0x62,
	0xc6, 0x44, 0x24, 0xfe, 0x63,
	0xc6, 0x44, 0x24, 0xff, 0x0a,
	0x48, 0xb8, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x48, 0xbf, 0x01, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x48, 0x8d, 0x74, 0x24, 0xfc,
	0x48, 0xba, 0x04, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
	0x0f, 0x05,
	0xc9, 
//	0xc3, 
};

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

	// attach to the process
	int res;
	res = ptrace(PTRACE_ATTACH, pid, 0, 0);
	if (res == -1) {
		printf("Failed to attach, err: %s\n", strerror(errno));
		return 1;
	}

	// wait for it
	res = waitpid(pid, NULL, WUNTRACED);
	if (res == -1) {
		printf("Failed to wait, err: %s\n", strerror(errno));
		return 1;
	}

	// write the shellcode
	for (size_t i = 0; i < sizeof (code); ++i) {
		res = ptrace(PTRACE_POKEDATA, pid, addr + i, code[i]);
		if (res == -1) {
			printf("Failed to write, i: %zu, err: %s\n", i, strerror(errno));
			return 1;
		}
	}

	// get the current registers
	struct user_regs_struct regs;
	res = ptrace(PTRACE_GETREGS, pid, 0, &regs);
	if (res == -1) {
		printf("Failed to get registers, err: %s\n", strerror(errno));
		return 1;
	}

	// modify the instruction pointer
	static const char syscall_ins[] = { 0x0f, 0x05 };

	regs.rip = addr + sizeof (syscall_ins);
	res = ptrace(PTRACE_SETREGS, pid, 0, &regs);
	if (res == -1) {
		printf("Failed to set registers, err: %s\n", strerror(errno));
		return 1;
	}

	res = ptrace(PTRACE_DETACH, pid, 0, 0);
	if (res == -1) {
		printf("Failed to detach, err: %s\n", strerror(errno));
		return 1;
	}

	return 0;
}
