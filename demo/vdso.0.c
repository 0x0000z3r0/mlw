#include <stdio.h>
#include <string.h>
#include <sys/auxv.h>
#include <elf.h>
#include <unistd.h>

int
main(int argc, char *argv[])
{
	printf("pid: %i\n", getpid());

	void *vdso;
	vdso = (void*)getauxval(AT_SYSINFO_EHDR);

	printf("vdso: %p\n", vdso);

	Elf64_Ehdr *ehdr;
	ehdr = vdso;

	Elf64_Shdr *shdr_sstr;
	shdr_sstr = ((Elf64_Shdr*)(vdso + ehdr->e_shoff) + ehdr->e_shstrndx);

	Elf64_Shdr *shdr;
	shdr = (Elf64_Shdr*)(vdso + ehdr->e_shoff);

	Elf64_Shdr *shdr_dstr;
	Elf64_Shdr *shdr_dsym;

	shdr_dstr = NULL;
	shdr_dsym = NULL;
	for (Elf64_Half i = 0; i < ehdr->e_shnum; ++i) {
		char *name;
		name = vdso + shdr_sstr->sh_offset + shdr->sh_name;

		printf("sec: %s\n", name);

		if (strcmp(name, ".dynsym") == 0) {
			shdr_dsym = shdr;
		} else if (strcmp(name, ".dynstr") == 0) {
			shdr_dstr = shdr;
		}

		++shdr;
	}

	if (shdr_dsym == NULL) {
		printf("failed to find the symbols table\n");
		return 1;
	}

	if (shdr_dstr == NULL) {
		printf("failed to find the strings section for symbols table\n");
		return 1;
	}

	Elf64_Sym *sym;
	sym = (Elf64_Sym*)(vdso + shdr_dsym->sh_offset);

	Elf64_Xword sym_cnt;
	sym_cnt = shdr_dsym->sh_size /  shdr_dsym->sh_entsize;
	for (Elf64_Xword i = 0; i < sym_cnt; ++i) {
		char *name;
		name = vdso + shdr_dstr->sh_offset + sym->st_name;

		printf("sym: %s\n", name);

		++sym;
	}

	return 0;
}
