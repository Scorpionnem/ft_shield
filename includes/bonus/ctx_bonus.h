/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx_bonus.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 11:09:25 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/25 10:51:19 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CTX_BONUS_H
# define CTX_BONUS_H

# include <server.h>
# include <sys/signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <netdb.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

# define SWORD_PORT 7003

typedef struct s_ctx
{
	t_server	server;
	int			log_fd;
}	t_ctx;

typedef enum e_log_type
{
	LOG_INFO,
	LOG_LOG,
	LOG_ERROR,
	LOG_NONE,
}	t_log_type;

#endif
