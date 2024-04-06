#ifndef _DEF_H_
#define _DEF_H_

#include <unistd.h>
#include <limits.h>
#include <stddef.h>
#include <sys/user.h>

// ============= MM =============

enum {
	MM_PROT_NONE  = 0,
	MM_PROT_READ,
	MM_PROT_WRITE,
	MM_PROT_EXEC,
	MM_PROT_PRIVATE
};

struct mm_entry {
	void *start;
	void *end;
	char prot;

	size_t off;

	int major;
	int minor;

	int inode;

	char path[PATH_MAX];
};

struct mm_list {
	size_t cap;
	size_t len;

	struct mm_entry *vec;
};

enum {
	MM_LIST_NEW_MEM	= -1,
	MM_LIST_NEW_OK	= 0
};

int
mm_list_new(struct mm_list*);

enum {
	MM_LIST_GET_PID		= -4,
	MM_LIST_GET_FMT		= -3,
	MM_LIST_GET_OPEN	= -2,
	MM_LIST_GET_MEM		= -1,
	MM_LIST_GET_OK		= 0,
};

int
mm_list_get(struct mm_list*, pid_t);

void
mm_list_del(struct mm_list*);

// ============= VM =============

enum {
	VM_READ_ERR	= -4,
	VM_READ_ATTACH	= -3,
	VM_READ_WAIT	= -2,
	VM_READ_DETACH	= -1,
	VM_READ_OK	= 0,
	VM_READ_PTRACE
};

int
vm_read(pid_t, size_t, void*, const void*, char);

enum {
	VM_WRITE_ERR	= -5,
	VM_WRITE_ATTACH	= -4,
	VM_WRITE_WAIT	= -3,
	VM_WRITE_POKE	= -2,
	VM_WRITE_DETACH	= -1,
	VM_WRITE_OK	= 0,
	VM_WRITE_PTRACE
};

int
vm_write(pid_t, size_t, const void*, const void*, char);

enum {
	VM_REGS_GET_ATTACH	= -4,
	VM_REGS_GET_WAIT	= -3,
	VM_REGS_GET_REGS	= -2,
	VM_REGS_GET_DETACH	= -1,
	VM_REGS_GET_OK		= 0,
};

int
vm_regs_get(pid_t, struct user_regs_struct*);

enum {
	VM_REGS_SET_ATTACH	= -4,
	VM_REGS_SET_WAIT	= -3,
	VM_REGS_SET_REGS	= -2,
	VM_REGS_SET_DETACH	= -1,
	VM_REGS_SET_OK		= 0,
};

int
vm_regs_set(pid_t, struct user_regs_struct*);

// ============= ELF =============

enum {
	ELF_VALID_MEM	= -1,
	ELF_VALID_OK	= 0,
	ELF_VALID_NO
};

int
elf_valid(size_t, void*);

#endif
