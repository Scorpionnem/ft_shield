/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 13:27:44 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/21 22:59:46 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"
#include <unistd.h>

int	daemonize(t_ctx *ctx)
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

	return (1);
}

#include <sys/file.h>

int	lock_file(t_ctx *ctx)
{
	ctx->lock_fd = open(LOCK_FILE, O_RDWR | O_CREAT, 0640);
	if (ctx->lock_fd < 0)
		return (0);
	if (flock(ctx->lock_fd, LOCK_EX | LOCK_NB) < 0)
		return (0);
	return (1);
}

int	unlock_file(t_ctx *ctx)
{
	flock(ctx->lock_fd, LOCK_UN);
	close(ctx->lock_fd);
	remove(LOCK_FILE);
	return (1);
}

int	ctx_init(t_ctx *ctx)
{
	printf(LOGIN_42 "\n");

	memset(ctx, 0, sizeof(t_ctx));
	ctx->running = true;
	signal(SIGINT, handle_sig);
	signal(SIGTERM, handle_sig);
	ctx->log_fd = open("ft_shield.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (ctx->log_fd == -1)
		return (0);
	logger_log(ctx, LOG_INFO, "Starting ft_shield");

	if (!lock_file(ctx))
	{
		logger_log(ctx, LOG_ERROR, "Failed to lock " LOCK_FILE);
		close(ctx->log_fd);
		return (0);
	}
	logger_log(ctx, LOG_INFO, "Locked " LOCK_FILE);

	if (!daemonize(ctx))
	{
		close(ctx->log_fd);
		close(ctx->lock_fd);
		return (0);
	}

	logger_log(ctx, LOG_INFO, "Opening server");
	if (!server_open(&ctx->server, SERVER_PORT))
	{
		logger_log(ctx, LOG_ERROR, "Failed to open server");
		close(ctx->log_fd);
		close(ctx->lock_fd);
		return (0);
	}
	server_set_message_hook(&ctx->server, message_hook, ctx);
	server_set_connect_hook(&ctx->server, connect_hook, ctx);
	server_set_disconnect_hook(&ctx->server, disconnect_hook, ctx);
	logger_log(ctx, LOG_INFO, "Server opened");
	return (1);
}

int	ctx_delete(t_ctx *ctx, bool log)
{
	if (log)
		logger_log(ctx, LOG_INFO, "Closing server");
	server_close(&ctx->server);
	if (log)
		logger_log(ctx, LOG_INFO, "Closing ft_shield");
	unlock_file(ctx);
	if (log)
		logger_log(ctx, LOG_INFO, "Unlocked " LOCK_FILE);
	close(ctx->log_fd);
	return (0);
}
