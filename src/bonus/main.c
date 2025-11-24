/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 11:03:41 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/24 12:00:45 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx_bonus.h"

# define SWORD_PORT 7003
int	g_sig = 0;

void	handle_sig(int sig)
{
	g_sig = sig;
}

void	msg_hook(t_client *client, char *msg, void *arg)
{
	(void)arg;(void)client;
	printf("\a%s\n", msg);
}

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int	main(void)
{
	t_ctx	ctx;

	printf("Waiting for ft_shield to be run somewhere\n");
	signal(SIGINT, handle_sig);
	server_open(&ctx.server, 7003);
	server_set_message_hook(&ctx.server, msg_hook, &ctx);
	while (g_sig == 0)
		server_update(&ctx.server);
	server_close(&ctx.server);
}
