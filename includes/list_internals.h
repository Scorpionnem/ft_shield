/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_internals.h                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 09:25:13 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/21 08:28:59 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_INTERNALS_H
# define LIST_INTERNALS_H

# include "list_config.h"
# include "list.h"

typedef struct	s_list_node
{
	struct s_list_node	*next;
	LIST_TYPE			data;
}	t_list_node;

int	list_node_size(t_list_node *nodes);

t_list_node	*list_node_last(t_list_node *nodes);

t_list_node	*list_node_new(LIST_TYPE data);

#endif
