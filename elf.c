#include "def.h"

#include <elf.h>
#include <string.h>

int
elf_valid(size_t len, void *buf)
{
	if (len < SELFMAG) 
		return ELF_VALID_MEM;

	int res;
	res = memcmp(buf, ELFMAG, SELFMAG);
	if (res == 0)
		return ELF_VALID_OK;

	return ELF_VALID_NO;
}
