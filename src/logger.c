/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 11:24:38 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/03 00:37:00 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

const char	*logger_get_log_header(t_log_type type)
{
	switch (type)
	{
		case LOG_INFO:
			return ("[INFO]");
		case LOG_LOG:
			return ("[LOG]");
		case LOG_ERROR:
			return ("[ERROR]");
		default:
			return ("[NONE]");
	}
}

void	logger_log_timestamp(int fd)
{
	time_t t = time(0);
	struct tm* tm = localtime(&t);
	char buf[64];

	strftime(buf, sizeof(buf), "[%d/%m/%Y-%H:%M:%S]", tm);
	dprintf(fd, "%s", buf);
}

int	get_sword_fd();

void	logger_log(t_ctx *ctx, t_log_type type, char *str, ...)
{
	(void)ctx;
	va_list	args;

	int fd = get_sword_fd();
	if (fd == -1)
		return ;
	va_start(args, str);
	logger_log_timestamp(fd);
	dprintf(fd, " %s ", logger_get_log_header(type));
	vdprintf(fd, str, args);
	va_end(args);
	dprintf(fd, "\n");
	close(fd);
}
