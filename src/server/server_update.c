/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_update.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 09:24:03 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/07 12:54:46 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_internals.h"
#include <sys/wait.h>


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
		server->total_bytes_received += size;
		client->buffer = server_strjoin(client->buffer, buffer);
		if (!client->buffer)
			return (0);
		if (server_strchr(client->buffer, '\n'))
			break ;
	}
	return (1);
}

# define TEXT_RED "\033[31m"
# define TEXT_GREEN "\033[32m"
# define TEXT_RESET "\033[0m"
# include <fcntl.h>

static int	server_read_client_raw(t_server *server, t_client *client)
{
	int	fd = open("transfer_out", O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (fd == -1)
		return (-1);

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
		write(fd, buffer, size);
		client->total_size += size;
		server->total_bytes_received += size;
		printf("\rReceived %ld/%ld bytes", client->total_size, client->file_size);
		fflush(stdout);
		if (client->total_size >= client->file_size)
		{
			printf(TEXT_GREEN "\nFinished receiving file\n" TEXT_RESET);
			break ;
		}
	}
	close(fd);
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
		{
			server->messages_received++;
			if (!server->message_hook(client, msg, strlen(msg), server->message_hook_arg))
			{
				free(msg);
				return (-1);
			}
		}
		free(msg);
	}
	return (1);
}

int	server_read_clients(t_server *server)
{
	t_client	**arr;
	int	i = 1;

	for (uint64_t c = 0; c < server->clients.size;)
	{
		arr = list_to_array(&server->clients);
		if (!arr)
			return (0);
		t_client	*client = arr[c];
		free(arr);

		if (server->fds[i].revents & POLLIN && client->shell_pid == 0)
		{
			if (server_read_client(server, client) == -1)
				continue ;

			int res = server_treat_client_input(server, client);
			if (res == 0)
				return (0);
			else if (res == -1)
				continue ;
		}

		c++;
		i++;
	}
	for (uint64_t c = 0; c < server->clients.size;)
	{
		arr = list_to_array(&server->clients);
		if (!arr)
			return (0);
		t_client	*client = arr[c];
		free(arr);

		if (client->shell_pid > 0)
		{
			int	status = 0;
			int result = waitpid(client->shell_pid, &status, WNOHANG);
			if (result == client->shell_pid)
			{
				server_send_to_fd(client->fd, PROMPT);
				client->shell_pid = 0;
				if (client->is_goofy_shell)
				{
					if (server->disconnect_hook)
						server->disconnect_hook(client, server->disconnect_hook_arg);
					close(client->fd);
					server_remove_client(server, client->fd);
					continue ;
				}
			}
		}
		c++;
	}
	return (1);
}

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

int	server_refresh_poll(t_server *server)
{
	t_client	**arr;

	arr = list_to_array(&server->clients);
	if (!arr)
		return (0);
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
