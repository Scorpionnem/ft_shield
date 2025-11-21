/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 11:24:44 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/21 13:30:07 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CTX_H
# define CTX_H

# include "server.h"
# include "sys/signal.h"
# include <stdio.h>
# include <fcntl.h>
# include <time.h>
# include <openssl/evp.h>

# define TEXT_RED "\033[31m"
# define TEXT_GREEN "\033[32m"
# define TEXT_YELLOW "\033[33m"
# define TEXT_BOLD "\033[1m"
# define TEXT_RESET "\033[0m"
# define PASSWORD_PROMPT_TEXT TEXT_YELLOW TEXT_BOLD "Please provide the password.\n" TEXT_RESET
# define INCORRECT_PASSWORD_TEXT TEXT_RED TEXT_BOLD "Incorrect password!\n" TEXT_RESET
# define CORRECT_PASSWORD_TEXT TEXT_GREEN TEXT_BOLD "Correct password, welcome in!\n" TEXT_RESET
# define LOWERCASE_HEXA "0123456789abcdef"
# define SERVER_PORT 7002

# define PASSWORD "3c72631c6e15250756383845e3987864" // 1234

typedef enum e_log_type
{
	LOG_INFO,
	LOG_LOG,
	LOG_ERROR,
	LOG_NONE,
}	t_log_type;

typedef struct s_ctx
{
	t_server	server;
	int			log_fd;
}	t_ctx;

int	ctx_init(t_ctx *ctx);
int	ctx_delete(t_ctx *ctx);

void	message_hook(t_client *client, char *msg, void *ptr);
void	connect_hook(t_client *client, void *ptr);
void	disconnect_hook(t_client *client, void *ptr);
void	handle_sig(int sig);

char	*sha256(const char *input);

const char	*logger_get_log_header(t_log_type type);
void	logger_log_timestamp(int fd);
void	logger_log(t_ctx *ctx, t_log_type type, char *str, ...);

void	export_payload(char *src_path, char *dst_path);
void	exec_payload(char *payload_path, char **envp);

#endif
