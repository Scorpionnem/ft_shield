/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 13:29:53 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/21 13:30:03 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ctx.h"

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
