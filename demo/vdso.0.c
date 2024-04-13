#include <stdio.h>
#include <string.h>
#include <sys/auxv.h>
#include <elf.h>
#include <unistd.h>
#include <time.h>

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

	const char sym_func[] = "time";
	printf("looking for '%s'\n", sym_func);

	Elf64_Sym *sym;
	sym = (Elf64_Sym*)(vdso + shdr_dsym->sh_offset);

	void *sym_ptr;
	sym_ptr = NULL;

	Elf64_Xword sym_cnt;
	sym_cnt = shdr_dsym->sh_size /  shdr_dsym->sh_entsize;
	for (Elf64_Xword i = 0; i < sym_cnt; ++i) {
		char *name;
		name = vdso + shdr_dstr->sh_offset + sym->st_name;

		static const char *type_strs[] = {
			"no type",
			"object",
			"function",
			"section",
			"file",
			"unknown"
		};

		unsigned char type;
		type = ELF64_ST_TYPE(sym->st_info);
		if (type > STT_FILE)
			type = sizeof (type_strs) - 1;

		printf("sym: %32s, type: (%u) %s\n", 
			name, ELF64_ST_TYPE(sym->st_info), type_strs[type]);

		if (strcmp(name, sym_func) == 0) {
			sym_ptr = vdso + sym->st_value;
		}

		++sym;
	}

	if (sym_ptr == NULL) {
		printf("could not find the function\n");
		return 1;
	} else {
		printf("ptr: %p\n", sym_ptr);
	}

	// check
	{
		void(*vdso_time)(time_t*) = sym_ptr;

		time_t time_vdso;
		vdso_time(&time_vdso);
		printf("vdso time: %s", ctime(&time_vdso));

		time_t time_reg;
		time(&time_reg);
		printf("regular time: %s", ctime(&time_reg));

		printf("are matching: %i\n", time_reg == time_vdso);
	}

	return 0;
}
