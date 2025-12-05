/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_internals.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 11:25:11 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/05 10:56:01 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_INTERNALS_H
# define SERVER_INTERNALS_H

# include "server.h"
# include <signal.h>

int	server_refresh_poll(t_server *server);
int	server_new_client(t_server *server);
int	server_read_clients(t_server *server);

int	server_remove_client(t_server *server, int fd);
int	server_add_client(t_server *server, int fd);

char	*server_strdup(const char *s);
char	*server_strjoin(char *s1, char *s2);
char	*server_strchr(char *s, char c);
char	*server_extract_line(char **str);

#endif
