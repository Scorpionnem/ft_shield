/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/17 12:26:03 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/21 08:54:10 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server_internals.h"

char	*server_strjoin(char *s1, char *s2)
{
	char	*dest;
	size_t	len;

	if (!s1)
		return (server_strdup(s2));
	len = (strlen(s1) + strlen(s2) + 1);
	dest = malloc(len * sizeof(char));
	if (dest == NULL)
		return (NULL);
	strcpy(dest, s1);
	strcat(dest, s2);
	free(s1);
	return ((char *)dest);
}

char	*server_strdup(const char *s)
{
	int		i;
	char	*dup;

	i = 0;
	dup = malloc((strlen(s) + 1) * sizeof(char));
	if (dup == NULL)
		return (NULL);
	while (s[i] != '\0')
	{
		dup[i] = s[i];
		i++;
	}
	dup[i] = '\0';
	return (dup);
}
