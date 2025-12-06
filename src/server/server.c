/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 11:20:35 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/06 14:06:23 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_internals.h"

int	server_open(t_server *server, int port)
{
	int yes = 1;

	memset(server, 0, sizeof(t_server));
	list_new(&server->clients);
	server->current_client_id = 1;
	server->socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server->socket_fd == -1)
		return (0);
	server->server_address.sin_family = AF_INET;
	server->server_address.sin_port = htons(port);
	server->server_address.sin_addr.s_addr = INADDR_ANY;
	if (setsockopt(server->socket_fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
	{
		close(server->socket_fd);
		return (0);
	}
	if (bind(server->socket_fd, (struct sockaddr*)&server->server_address, sizeof(server->server_address)) == -1)
	{
		close(server->socket_fd);
		return (0);
	}
	if (listen(server->socket_fd, MAX_CLIENTS) == -1)
	{
		close(server->socket_fd);
		return (0);
	}
	return (1);
}

static void close_client(t_client *cl)
{
	if (cl->buffer)
		free(cl->buffer);
	close(cl->fd);
}

static void kill_client_shell(t_client *cl)
{
	if (cl->shell_pid > 0)
		kill(cl->shell_pid, SIGKILL);
}

int	server_close(t_server *server, bool kill_shells)
{
	if (kill_shells)
		list_for_each(&server->clients, kill_client_shell);
	list_for_each(&server->clients, close_client);
	list_delete(&server->clients, true);
	if (server->socket_fd != 0)
		close(server->socket_fd);
	return (1);
}

void	server_set_message_hook(t_server *server, void (*func)(t_client *client, char *msg, int64_t size, void *arg), void *arg)
{
	server->message_hook = func;
	server->message_hook_arg = arg;
}

void	server_set_disconnect_hook(t_server *server, void (*func)(t_client *client, void *arg), void *arg)
{
	server->disconnect_hook = func;
	server->disconnect_hook_arg = arg;
}

void	server_set_connect_hook(t_server *server, void (*func)(t_client *client, void *arg), void *arg)
{
	server->connect_hook = func;
	server->connect_hook_arg = arg;
}

int	server_send_to_fd(int fd, const char *msg)
{
	send(fd, msg, strlen(msg), 0);
	return (1);
}

int	server_send_to_all(t_server *server, const char *msg)
{
	t_client	**arr;

	arr = list_to_array(&server->clients);
	for (uint64_t c = 0; c < server->clients.size; c++)
		server_send_to_fd(arr[c]->fd, msg);
	free(arr);
	return (1);	
}

int	server_send_to_id(t_server *server, int id, const char *msg)
{
	t_client	**arr;

	arr = list_to_array(&server->clients);
	for (uint64_t c = 0; c < server->clients.size; c++)
		if (arr[c]->id == id)
		{
			send(arr[c]->fd, msg, strlen(msg), 0);
			break ;
		}
	free(arr);
	return (1);
}
