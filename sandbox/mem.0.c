#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <linux/limits.h>
#include <string.h>
#include <errno.h>
#include <sys/uio.h>
#include <sys/types.h>
#include <elf.h>
#include <sys/stat.h>

extern ssize_t 
process_vm_readv(pid_t pid,
		const struct iovec *local_iov,
                unsigned long liovcnt,
                const struct iovec *remote_iov,
                unsigned long riovcnt,
                unsigned long flags);

int
main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Supply a PID\n");
		goto _EXIT;
	}

	pid_t pid;
	pid = atoi(argv[1]);
	printf("pid: %i\n", pid);

	int res;

	char path[PATH_MAX];
	res = snprintf(path, sizeof (path), "/proc/%i/maps", pid);
	if (res < 0) {
		printf("Failed to produce the file path\n");
		goto _EXIT;
	}

	printf("path: %s\n", path);

	int file;
	file = open(path, O_RDONLY, S_IRUSR);
	if (file == -1) {
		printf("Failed to open the file, err: %s\n", strerror(errno));
		goto _EXIT;
	}

	size_t line_cap;
	line_cap = 4096;

	char *line;
	line = malloc(line_cap);
	if (line == NULL) {
		printf("Failed to allocate for a line, err: %s\n", strerror(errno));
		goto _CLOSE;
	}

	off_t off;
	off = 0;		
	while (1) {
		ssize_t bytes;
		bytes = 0;

		char *pos;
		pos = NULL;
		while (1) {
			bytes = pread(file, line, line_cap, off);
			if (bytes == -1) {
				printf("Failed to read the line, err: %s\n", strerror(errno));
				goto _FREE;
			}

			if (bytes == 0) {
				printf("End of file\n");
				break;
			}

			pos = strchr(line, '\n');
			if (pos == NULL) {
				printf("Could not find newline, increasing the buffer to %zu\n", line_cap);
				off += line_cap;
				line_cap *= 2;

				char *tmp;
				tmp = realloc(line, line_cap);
				if (tmp == NULL) {
					printf("Failed to reallocate the line buffer, err: %s\n", strerror(errno));
					goto _FREE;
				}

				line = tmp;
			}

			off += (off_t)(pos - line + 1);
			break;
		}

		if (bytes == 0) {
			break;
		}

		void *vas;
		void *vae;

		vas = NULL;
		vae = NULL;

		char prot[4];
		bzero(prot, sizeof (prot));

		int pao;
		pao = 0;

		int maj;
		int min;

		maj = 0;
		min = 0;

		int inode;
		inode = 0;

		char mod[PATH_MAX];
		bzero(mod, sizeof (mod));

		int sno;
		sno = 0;
	
		const int sno_max = 74;

		sscanf(line, "%p-%p %c%c%c%c %x %u:%u %i %s%n", 
			&vas, &vae,
			&prot[0], &prot[1], &prot[2], &prot[3],
			&pao,
			&maj, &min,
			&inode,
			mod,
			&sno);

		if (sno == sno_max)
			mod[0] = '\0';

		char sig[EI_NIDENT];
		bzero(sig, sizeof (sig));

		int is_elf;
		is_elf = 0;

		char *reason;
		reason = "none";
		if (mod[0] != '\0' && prot[0] == 'r') {
			struct stat stat;
			res = lstat(mod, &stat);
			if (res != -1 && S_ISREG(stat.st_mode)) {
				struct iovec local[1];
				local[0].iov_base = sig;
				local[0].iov_len  = sizeof (sig);

				struct iovec remote[1];
				remote[0].iov_base = vas;
				remote[0].iov_len  = sizeof (sig);
				bytes = process_vm_readv(pid, local, 1, remote, 1, 0);
				if (bytes == -1) {
					printf("Failed to read from the process memory for the signature, err: %s\n", strerror(errno));
					goto _FREE;
				}

				is_elf = memcmp(sig, ELFMAG, SELFMAG) == 0;
				if (!is_elf) {
					reason = "failed signature check";
				}
			} else {
				reason = "failed to stat or is not a regular file";
			}
		} else {
			reason = "no module name or missing read protection";
		}


		printf("\nvas:  %p\n"
			"vae:  %p\n"
			"prot: %c%c%c%c\n"
			"off:  0x%08x\n"
			"mod:  %s\n"
			"sig:  %02x%02x%02x%02x\n"
			"elf:  %i (%s)\n", 
			vas, vae, 
			prot[0], prot[1], prot[2], prot[3],
			pao,
			mod,
			sig[0], sig[1], sig[2], sig[3],
			is_elf, reason);

		if (is_elf) {
			int class64;
			class64 = sig[EI_CLASS] == ELFCLASS64;

			if (!class64) {
				printf("32 bit elf binaries are currently not supported\n");
				goto _FREE;
			}

			Elf64_Ehdr ehdr;
			bzero(&ehdr, sizeof (ehdr));

			struct iovec local[1];
			local[0].iov_base = &ehdr;
			local[0].iov_len  = sizeof (ehdr);

			struct iovec remote[1];
			remote[0].iov_base = vas;
			remote[0].iov_len  = local[0].iov_len;
			bytes = process_vm_readv(pid, local, 1, remote, 1, 0);
			if (bytes == -1) {
				printf("Failed to read from the process memory for the elf header, err: %s\n", strerror(errno));
				goto _FREE;
			}

			printf("  entry: 0x%016lx\n"
				"  type: 0x%04x (exe: %02x, lib: %02X)\n", 
				ehdr.e_entry,
				ehdr.e_type, ET_EXEC, ET_DYN);
		}
	}

_FREE:
	free(line);
_CLOSE:
	close(file);
_EXIT:
	return 0;
}
