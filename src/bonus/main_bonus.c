/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 11:03:41 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/07 12:44:43 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx_bonus.h"

int	g_sig = 0;

void	handle_sig(int sig)
{
	g_sig = sig;
}

int	msg_hook(t_client *client, char *msg, int64_t size, void *arg)
{
	(void)arg;
	if (client->receiving_file)
	{
		int	fd = open("transfer_out", O_CREAT | O_WRONLY | O_TRUNC, 0777);
		if (fd == -1)
			return 1;

		write(fd, (unsigned char*)msg, size);
		close(fd);
	}
	else
		printf("\a%s\n", msg);
	return (1);
}

int	main(void)
{
	t_ctx	ctx;

	printf("Waiting for ft_shield to be run somewhere\n");
	signal(SIGINT, handle_sig);
	if (!server_open(&ctx.server, SWORD_PORT))
		return (1);
	server_set_message_hook(&ctx.server, msg_hook, &ctx);
	while (g_sig == 0)
		server_update(&ctx.server);
	server_close(&ctx.server, true);
}
