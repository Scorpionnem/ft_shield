/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_node.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 09:27:20 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/21 09:43:16 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list_internals.h"

int	list_node_size(t_list_node *nodes)
{
	int	count;

	count = 0;
	while (nodes != NULL)
	{
		count++;
		nodes = nodes->next;
	}
	return (count);
}

t_list_node	*list_node_last(t_list_node *nodes)
{
	int	count;

	count = list_node_size(nodes);
	while (count > 1)
	{
		nodes = nodes->next;
		count--;
	}
	return (nodes);	
}

t_list_node	*list_node_new(LIST_TYPE data)
{
	t_list_node	*new;

	new = malloc(sizeof(t_list_node));
	if (!new)
		return (NULL);
	memset(new, 0, sizeof(t_list_node));
	new->data = data;
	return (new);
}
