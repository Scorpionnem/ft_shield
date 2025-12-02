/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_internals.h                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 11:25:11 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/02 20:45:00 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_INTERNALS_H
# define SERVER_INTERNALS_H

# include "server.h"

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
