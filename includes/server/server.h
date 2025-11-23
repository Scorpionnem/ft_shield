/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 18:15:07 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/23 09:45:59 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_H
# define SERVER_H

# include <arpa/inet.h>
# include <netinet/in.h>
# include <sys/socket.h>
# include <unistd.h>
# include <stdlib.h>
# include <poll.h>
# include <string.h>
# include <errno.h>
# include <stdio.h>
# include <stdbool.h>

# define MAX_CLIENTS 3
#  if MAX_CLIENTS <= 0
#   error "MAX_CLIENTS should be > 0"
#  endif

# include "list.h"

# define PROMPT "$> "

typedef struct s_client
{
	int		fd;
	int		id;

	bool	logged;
	int		shell_pid;
}	t_client;

typedef struct s_server
{
	int					socket_fd;
	struct sockaddr_in	server_address;
	struct 	pollfd		fds[MAX_CLIENTS + 2];

	unsigned int		current_client_id;
	t_list				clients;

	void (*connect_hook)(t_client *, void *);
	void	*connect_hook_arg;
	void (*disconnect_hook)(t_client *, void *);
	void	*disconnect_hook_arg;
	void (*message_hook)(t_client *client, char *msg, void *arg);
	void	*message_hook_arg;
}	t_server;

int	server_update(t_server *server);
int	server_close(t_server *server);
int	server_open(t_server *server, int port);

void	server_set_connect_hook(t_server *server, void (*func)(t_client *client, void *arg), void *arg);
void	server_set_disconnect_hook(t_server *server, void (*func)(t_client *client, void *arg), void *arg);
void	server_set_message_hook(t_server *server, void (*func)(t_client *client, char *msg, void *arg), void *arg);

int	server_send_to_fd(int fd, const char *msg);
int	server_send_to_id(t_server *server, int id, const char *msg);

int	server_send_to_all(t_server *server, const char *msg);

#endif
