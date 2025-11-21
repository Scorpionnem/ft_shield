/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 13:27:44 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/21 13:29:30 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

int	ctx_init(t_ctx *ctx)
{
	printf("mbatty\n");

	memset(ctx, 0, sizeof(t_ctx));
	signal(SIGINT, handle_sig);

	ctx->log_fd = open("ft_shield.log", O_WRONLY | O_CREAT | O_APPEND, 0644);
	if (ctx->log_fd == -1)
		return (0);
	logger_log(ctx, LOG_INFO, "Starting ft_shield");

	logger_log(ctx, LOG_INFO, "Opening server");
	if (!server_open(&ctx->server, SERVER_PORT))
	{
		logger_log(ctx, LOG_ERROR, "Failed to open server");
		close(ctx->log_fd);
		return (0);
	}
	server_set_message_hook(&ctx->server, message_hook, ctx);
	server_set_connect_hook(&ctx->server, connect_hook, ctx);
	server_set_disconnect_hook(&ctx->server, disconnect_hook, ctx);
	logger_log(ctx, LOG_INFO, "Server opened");
	return (1);
}

int	ctx_delete(t_ctx *ctx)
{
	logger_log(ctx, LOG_INFO, "Closing server");
	server_close(&ctx->server);
	logger_log(ctx, LOG_INFO, "Closing ft_shield");
	close(ctx->log_fd);
	return (0);
}
