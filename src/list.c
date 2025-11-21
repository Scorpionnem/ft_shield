/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 09:26:34 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/21 10:14:02 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "list_internals.h"

int	list_new(t_list *vec)
{
	memset(vec, 0, sizeof(t_list));
	return (1);
}

int	list_delete(t_list *vec, bool free_data)
{
	t_list_node	*tmp;

	(void)free_data;
	while (vec->list)
	{
		#if LIST_CAN_FREE
			if (free_data)
				free(vec->list->data);
		#endif
		tmp = vec->list->next;
		free(vec->list);
		vec->list = tmp;
	}
	vec->size = 0;
	vec->list = NULL;
	return (1);
}

int	list_add_back(t_list *vec, LIST_TYPE data)
{
	t_list_node	*last;
	t_list_node	*new;

	last = list_node_last(vec->list);
	new = list_node_new(data);
	if (!new)
		return (0);
	if (!last)
		vec->list = new;
	else
		last->next = new;
	vec->size = list_node_size(vec->list);
	return (1);
}

int	list_add_front(t_list *vec, LIST_TYPE data)
{
	t_list_node	*new;

	new = list_node_new(data);
	if (!new)
		return (0);
	new->next = vec->list;
	vec->list = new;
	vec->size = list_node_size(vec->list);
	return (1);
}

LIST_TYPE	list_last(t_list *vec)
{
	t_list_node	*node;

	node = list_node_last(vec->list);
	if (!node)
		return (0);
	return (node->data);
}

int	list_delete_node(t_list *vec, LIST_TYPE data, bool (*cmp_func)(LIST_TYPE, LIST_TYPE), bool free_data)
{
	t_list_node	*list;
	t_list_node	*prev;
	t_list_node	*next;

	(void)free_data;
	list = vec->list;
	prev = NULL;
	while (list)
	{
		if (cmp_func(data, list->data))
		{
			next = list->next;
			#if LIST_CAN_FREE
				if (free_data)
					free(list->data);
					#endif
			free(list);
			if (!prev)
				vec->list = next;
			else
				prev->next = next;
			vec->size = list_node_size(vec->list);
			return (1);
		}
		prev = list;
		list = list->next;
	}
	vec->size = list_node_size(vec->list);
	return (0);
}

void	list_for_each(t_list *vec, void (*func)(LIST_TYPE))
{
	t_list_node	*list;
	
	list = vec->list;
	while (list)
	{
		func(list->data);
		list = list->next;
	}
}

void		list_for_each_param(t_list *vec, void (*func)(LIST_TYPE, void *), void *param)
{
	t_list_node	*list;
	
	list = vec->list;
	while (list)
	{
		func(list->data, param);
		list = list->next;
	}
}

bool	list_has(t_list *vec, LIST_TYPE data, bool (*cmp_func)(LIST_TYPE, LIST_TYPE))
{
	t_list_node	*list;
	
	list = vec->list;
	while (list)
	{
		if (cmp_func(data, list->data))
			return (true);
		list = list->next;
	}
	return (false);
}

LIST_TYPE	*list_to_array(t_list *vec)
{
	LIST_TYPE	*res;
	t_list_node	*list;
	int			i;
	
	res = malloc(vec->size * sizeof(LIST_TYPE));
	if (!res)
		return (0);
	list = vec->list;
	i = 0;
	while (list)
	{
		res[i] = list->data;
		list = list->next;
		i++;
	}
	return (res);
}
