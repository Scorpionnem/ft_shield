/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_clients.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/04 09:29:59 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/04 14:33:25 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"

static int	server_add_client(t_server *server, int fd)
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
		close(fd);
		free(cl);
		return (0);
	}
	list_add_back(&server->clients, cl);
	if (server->connect_hook)
		server->connect_hook(cl, server->connect_hook_arg);
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
	close(fd);
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
