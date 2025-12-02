/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 11:03:41 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/02 20:48:55 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx_bonus.h"

int	g_sig = 0;

void	handle_sig(int sig)
{
	g_sig = sig;
}

char	*server_strjoin(char *s1, char *s2);
char	*server_strdup(const char *s);

void	msg_hook(t_client *client, char *msg, void *arg)
{
	(void)arg;(void)client;
	printf("\a%s\n", msg);
}

int	main(void)
{
	t_ctx	ctx;

	printf("Waiting for ft_shield to be run somewhere\n");
	signal(SIGINT, handle_sig);
	server_open(&ctx.server, SWORD_PORT);
	server_set_message_hook(&ctx.server, msg_hook, &ctx);
	while (g_sig == 0)
		server_update(&ctx.server);
	server_close(&ctx.server);
}
