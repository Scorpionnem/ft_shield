/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 13:28:48 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/21 13:35:45 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

static int	check_client_password(t_ctx *ctx, t_client *client, char *msg)
{
	if (!client->logged)
	{
		char *hashed_msg = sha256(msg);
		if (!strcmp(hashed_msg, PASSWORD))
		{
			logger_log(ctx, LOG_LOG, "Client %d correct password (%s)", client->id, hashed_msg);
			server_send_to_id(&ctx->server, client->id, CORRECT_PASSWORD_TEXT);
			client->logged = true;
			free(hashed_msg);
			return (0);
		}
		logger_log(ctx, LOG_LOG, "Client %d wrong password (%s)", client->id, hashed_msg);
		server_send_to_id(&ctx->server, client->id, INCORRECT_PASSWORD_TEXT);
		free(hashed_msg);
		return (0);
	}
	return (1);
}

void	message_hook(t_client *client, char *msg, void *ptr)
{
	t_ctx	*ctx = ptr;

	if (!check_client_password(ctx, client, msg))
		return ;

	logger_log(ctx, LOG_LOG, "From %d: %s", client->id, msg);
}

void	connect_hook(t_client *client, void *ptr)
{
	t_ctx	*ctx = ptr;

	server_send_to_id(&ctx->server, client->id, "Welcome to server!\n");
	server_send_to_id(&ctx->server, client->id, PASSWORD_PROMPT_TEXT);
	logger_log(ctx, LOG_INFO, "Client %d joined", client->id);
}

void	disconnect_hook(t_client *client, void *ptr)
{
	t_ctx	*ctx = ptr;

	logger_log(ctx, LOG_INFO, "Client %d left", client->id);
}
