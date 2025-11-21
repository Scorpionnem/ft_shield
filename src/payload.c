/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   payload.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 13:27:21 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/21 13:27:24 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

void	export_payload(char *src_path, char *dst_path)
{
	char 	buf[4096];
	ssize_t rdb;
	int		fdin;
	int		fdout;

	fdin = open(src_path, O_RDONLY);
	fdout = open(dst_path, O_CREAT | O_WRONLY | O_TRUNC, 0777);

	do
	{
		rdb = read(fdin, buf, sizeof(buf));
		write(fdout, buf, rdb);
	} while (rdb > 0);

	close(fdin);
	close(fdout);
}

void	exec_payload(char *payload_path, char **envp)
{
	char	*argv[] = {payload_path, NULL};
	execve(payload_path, argv, envp);
	exit(0);
}
