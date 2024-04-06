#include "def.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <signal.h>

int
mm_list_new(struct mm_list *mm_list)
{
	mm_list->cap = 16;
	mm_list->len = 0;
	mm_list->vec = malloc(sizeof (struct mm_entry) * mm_list->cap);
	if (mm_list->vec == NULL) {
		return MM_LIST_NEW_MEM;
	}

	return MM_LIST_NEW_OK;
}

int
mm_list_get(struct mm_list *mm_list, pid_t pid)
{
	int res;

	res = kill(pid, 0);
	if (res == -1) {
		return MM_LIST_GET_PID;
	}

	char path[PATH_MAX];
	res = snprintf(path, sizeof (path), "/proc/%i/maps", pid);
	if (res < 0) {
		return MM_LIST_GET_FMT;
	}

	FILE *file;
	file = fopen(path, "r");
	if (file == NULL) {
		return MM_LIST_GET_OPEN;
	}

	char *line;
	size_t len;
	ssize_t read;
		
	read = getline(&line, &len, file);
	while (read != -1) {
		if (mm_list->len >= mm_list->cap) {
			mm_list->cap *= 2;

			struct mm_entry *list;
			list = realloc(mm_list->vec, sizeof (struct mm_entry) * mm_list->cap);
			if (list == NULL) {
				free(line);
				fclose(file);
				return MM_LIST_GET_MEM;
			}

			mm_list->vec = list;
		}

		struct mm_entry *mm_entry;
		mm_entry = &mm_list->vec[mm_list->len];
		bzero(mm_entry, sizeof (struct mm_entry));

		char prot[4];
		bzero(prot, sizeof (prot));

		int count;
		sscanf(line, "%p-%p %c%c%c%c %lx %u:%u %i %s%n", 
			&mm_entry->start, &mm_entry->end,
			&prot[0], &prot[1], &prot[2], &prot[3],
			&mm_entry->off,
			&mm_entry->major, &mm_entry->minor,
			&mm_entry->inode,
			mm_entry->path,
			&count);

		if (prot[0] == 'r')
			mm_entry->prot |= (1 << MM_PROT_READ);
		if (prot[1] == 'w')
			mm_entry->prot |= (1 << MM_PROT_WRITE);
		if (prot[2] == 'x')
			mm_entry->prot |= (1 << MM_PROT_EXEC);
		if (prot[3] == 'p')
			mm_entry->prot |= (1 << MM_PROT_PRIVATE);

		const int count_max = 74;
		if (count == count_max) {
			mm_entry->path[0] = '\0';
		}

		++mm_list->len;
		read = getline(&line, &len, file);
	}

	free(line);
	fclose(file);
	return MM_LIST_GET_OK;
}

void
mm_list_del(struct mm_list *mm_list)
{
	free(mm_list->vec);
}
