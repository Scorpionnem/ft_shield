/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 11:20:35 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/03 19:40:54 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_internals.h"
#include <sys/wait.h>

int	server_update(t_server *server)
{
	server_refresh_poll(server);
	int	poll_events = poll(server->fds, server->clients.size + 1, 250);
	if (poll_events == -1 && errno == EINTR)
		return (1);
	if ((server->fds[0].revents & POLLIN) != 0)
		server_new_client(server);
	server_read_clients(server);
	return (1);
}

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

#include <signal.h>

static void close_client(t_client *cl)
{
	if (cl->shell_pid > 0)
		kill(cl->shell_pid, SIGKILL);
	if (cl->buffer)
		free(cl->buffer);
	close(cl->fd);
}

int	server_close(t_server *server)
{
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

static bool	cmp_client(t_client *c1, t_client *c2)
{
	return (c1->fd == c2->fd);
}

int	server_remove_client(t_server *server, int fd)
{
	t_client	cl;

	cl.fd = fd;
	t_client *real_cl = list_find(&server->clients, &cl, cmp_client);
	if (real_cl && real_cl->buffer)
		free(real_cl->buffer);
	list_delete_node(&server->clients, &cl, cmp_client, true);
	return (1);
}

int	server_add_client(t_server *server, int fd)
{
	t_client	*cl;

	cl = malloc(sizeof(t_client));
	if (!cl)
		return (0);
	memset(cl, 0, sizeof(t_client));
	cl->fd = fd;
	cl->id = server->current_client_id++;
	cl->logged = false;
	if (server->clients.size >= MAX_CLIENTS)
	{
		server_send_to_fd(fd, "Cant connect you rn\n");
		free(cl);
		close(fd);
		return (0);
	}
	list_add_back(&server->clients, cl);
	if (server->connect_hook)
		server->connect_hook(cl, server->connect_hook_arg);
	return (1);
}

int	server_refresh_poll(t_server *server)
{
	t_client	**arr;

	arr = list_to_array(&server->clients);
	server->fds[0].fd = server->socket_fd;
	server->fds[0].events = POLLIN;
	server->fds[0].revents = 0;
	int	i = 1;
	for (uint64_t c = 0; c < server->clients.size; c++)
	{
		server->fds[i].fd = arr[c]->fd;
		server->fds[i].events = POLLIN;
		server->fds[i].revents = 0;
		i++;
	}
	free(arr);
	return (1);
}

int	server_new_client(t_server *server)
{
	struct sockaddr_in		addr;
	unsigned int			len = sizeof(struct sockaddr_in);
	char					ip[INET_ADDRSTRLEN];
	int						fd;

	memset(&addr, 0, len);
	fd = accept(server->socket_fd, (struct sockaddr*)&addr, &len);
	if (fd == -1)
		return (0);

	inet_ntop(AF_INET, &addr, ip, INET_ADDRSTRLEN);
	server_add_client(server, fd);
	return (1);
}

char	*append_to_str(char *s1, char *s2, size_t s1len, size_t s2len)
{
	if (!s1)
	{
		char	*res = calloc(s2len, sizeof(char));
		if (!res)
			return (NULL);
		memcpy(res, s2, s2len);
		return (res);
	}
	char	*res = calloc(s1len + s2len, sizeof(char));
	if (!res)
		return (NULL);
	memcpy(res, s1, s1len);
	memcpy(res + s1len, s2, s2len);
	free(s1);
	return (res);
}

static int	server_read_client_nl(t_server *server, t_client *client)
{
	while (1)
	{
		char 	buffer[1024] = {0};
		ssize_t size;

		size = recv(client->fd, buffer, sizeof(buffer), 0);
		if (size == 0 || size == -1)
		{
			if (server->disconnect_hook)
				server->disconnect_hook(client, server->disconnect_hook_arg);
			server_remove_client(server, client->fd);
			return (-1);
		}
		client->buffer = server_strjoin(client->buffer, buffer);
		if (server_strchr(client->buffer, '\n'))
			break ;
	}
	return (1);
}

# define TEXT_RED "\033[31m"
# define TEXT_GREEN "\033[32m"
# define TEXT_RESET "\033[0m"

static int	server_read_client_raw(t_server *server, t_client *client)
{
	while (1)
	{
		char 	buffer[8192] = {0};
		ssize_t size;

		size = recv(client->fd, buffer, sizeof(buffer), 0);
		if (size == 0 || size == -1)
		{
			if (server->disconnect_hook)
				server->disconnect_hook(client, server->disconnect_hook_arg);
			server_remove_client(server, client->fd);
			printf(TEXT_RED "\nFailed to fully receive raw data\n" TEXT_RESET);
			return (-1);
		}
		client->buffer = append_to_str(client->buffer, buffer, client->total_size, size);
		client->total_size += size;
		printf("\rReceived %ld/%ld bytes", client->total_size, client->file_size);
		fflush(stdout);
		if (client->total_size >= client->file_size)
		{
			printf(TEXT_GREEN "\nFinished receiving file\n" TEXT_RESET);
			break ;
		}
	}
	return (1);
}

static int	server_read_client(t_server *server, t_client *client)
{
	if (client->receiving_file)
		return (server_read_client_raw(server, client));
	else
		return (server_read_client_nl(server, client));
}

static int	server_treat_client_input(t_server *server, t_client *client)
{
	while (1)
	{
		if (client->receiving_file)
		{
			printf("Total size %ld\n", client->total_size);
			if (server->message_hook)
				server->message_hook(client, client->buffer, client->file_size, server->message_hook_arg);
			client->receiving_file = false;
			free(client->buffer);	
			client->buffer = NULL;
			break ;
		}
		char	*msg = server_extract_line(&client->buffer);
		if (!msg)
			break ;

		if (!strncmp(msg, "transfer:", 9))
		{
			client->receiving_file = true;
			client->file_size = atoll(msg + 9);
			client->total_size = 0;
			printf("Received file transfer size %ld\n", client->file_size);
			free(msg);
			break ;
		}

		if (server->message_hook)
			server->message_hook(client, msg, strlen(msg), server->message_hook_arg);
		free(msg);
	}
	return (1);
}

int	server_read_clients(t_server *server)
{
	t_client	**arr;
	int	i = 1;

	// arr = list_to_array(&server->clients);
	for (uint64_t c = 0; c < server->clients.size;)
	{
		arr = list_to_array(&server->clients);
		t_client	*client = arr[c];
		free(arr);

		if (server->fds[i].revents & POLLIN && client->shell_pid == 0)
		{
			if (server_read_client(server, client) == -1)
				continue ;

			if (!server_treat_client_input(server, client))
				return (0);
		}

		c++;
		i++;
	}
	arr = list_to_array(&server->clients);
	for (uint64_t c = 0; c < server->clients.size; c++)
	{
		if (arr[c]->shell_pid > 0)
		{
			int	status = 0;
			int result = waitpid(arr[c]->shell_pid, &status, WNOHANG);
			if (result == arr[c]->shell_pid)
				arr[c]->shell_pid = 0;
		}
	}
	free(arr);
	return (1);
}
