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

// ============= PRC =============

enum {
	PRC_LOCK_ATTACH	= -3,
	PRC_LOCK_WAIT	= -2,
	PRC_LOCK_SIG	= -1,
	PRC_LOCK_OK	= 0,
};

int
prc_lock(pid_t);

enum {
	PRC_CONT_ERR	= -3,
	PRC_CONT_WAIT	= -2,
	PRC_CONT_SIG	= -1,
	PRC_CONT_OK	= 0,
};

int
prc_cont(pid_t);

enum {
	PRC_UNLOCK_DETACH	= -1,
	PRC_UNLOCK_OK	= 0,
};

int
prc_unlock(pid_t);

// ============= VM =============

enum {
	VM_READ_PEEK	= -1,
	VM_READ_OK	= 0,
};

int
vm_read(pid_t, size_t, void*, const void*);

enum {
	VM_WRITE_POKE	= -1,
	VM_WRITE_OK	= 0,
};

int
vm_write(pid_t, size_t, const void*, const void*);

enum {
	VM_REGS_GET_REG	= -1,
	VM_REGS_GET_OK	= 0,
};

int
vm_regs_get(pid_t, struct user_regs_struct*);

enum {
	VM_REGS_SET_REG	= -1,
	VM_REGS_SET_OK	= 0,
};

int
vm_regs_set(pid_t, struct user_regs_struct*);

#endif
