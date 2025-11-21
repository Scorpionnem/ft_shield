/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/16 18:15:16 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/21 09:56:42 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.h"
#include "sys/signal.h"

int	g_sig = 0;

void	handle_sig(int sig)
{
	g_sig = sig;
}

# define TEXT_RED "\033[31m"
# define TEXT_GREEN "\033[32m"
# define TEXT_YELLOW "\033[33m"
# define TEXT_BOLD "\033[1m"
# define TEXT_RESET "\033[0m"
# define PASSWORD_PROMPT_TEXT TEXT_YELLOW TEXT_BOLD "Please provide the password.\n" TEXT_RESET
# define INCORRECT_PASSWORD_TEXT TEXT_RED TEXT_BOLD "Incorrect password!\n" TEXT_RESET
# define CORRECT_PASSWORD_TEXT TEXT_GREEN TEXT_BOLD "Correct password, welcome in!\n" TEXT_RESET

# define LOWERCASE_HEXA "0123456789abcdef"

# define PASSWORD "3c72631c6e15250756383845e3987864" // 1234

# include <openssl/evp.h>

char	*sha256(const char *input)
{
	EVP_MD_CTX* context = EVP_MD_CTX_new();
	if (!context)
		return (NULL);

	const EVP_MD* md = EVP_sha256();
	if (!EVP_DigestInit_ex(context, md, NULL))
		return (NULL);

	if (!EVP_DigestUpdate(context, input, strlen(input)))
		return (NULL);

	unsigned char hash[EVP_MAX_MD_SIZE];
	unsigned int lengthOfHash = 0;

	if (!EVP_DigestFinal_ex(context, hash, &lengthOfHash))
		return (NULL);

	EVP_MD_CTX_free(context);

	char	*res = calloc(lengthOfHash + 1, sizeof(char));
	if (!res)
		return (NULL);

	for (unsigned int i = 0; i < lengthOfHash; i++)
		res[i] = LOWERCASE_HEXA[hash[i] % 16];

	return (res);
}

int	check_client_password(t_server *server, t_client *client, char *msg)
{
	if (!client->logged)
	{
		char *hashed_msg = sha256(msg);
		int	cmp = strcmp(hashed_msg, PASSWORD);
		free(hashed_msg);
		if (!cmp)
		{
			server_send_to_id(server, client->id, CORRECT_PASSWORD_TEXT);
			client->logged = true;
			return (1);
		}
		server_send_to_id(server, client->id, INCORRECT_PASSWORD_TEXT);
		return (0);
	}
	return (1);
}

void	message_hook(t_client *client, char *msg, void *ptr)
{
	t_server	*server = ptr;

	if (!check_client_password(server, client, msg))
		return ;

	printf("From %d: %s\n", client->id, msg);
}

void	connect_hook(t_client *client, void *ptr)
{
	t_server	*server = ptr;

	server_send_to_id(server, client->id, "Welcome to server!\n");
	server_send_to_id(server, client->id, PASSWORD_PROMPT_TEXT);
	printf("Client %d joined\n", client->id);
}

void	disconnect_hook(t_client *client, void *ptr)
{
	(void)client;(void)ptr;
	printf("Client %d left\n", client->id);
}

int	main(void)
{
	t_server	server;

	signal(SIGINT, handle_sig);
	if (!server_open(&server, 7002))
		return (1);
	server_set_message_hook(&server, message_hook, &server);
	server_set_connect_hook(&server, connect_hook, &server);
	server_set_disconnect_hook(&server, disconnect_hook, &server);
	while (g_sig == 0)
		server_update(&server);
	server_close(&server);
	return (0);
}
