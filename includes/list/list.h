/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/20 09:25:29 by mbatty            #+#    #+#             */
/*   Updated: 2025/12/02 20:12:58 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_H
# define LIST_H

# include <stdlib.h>
# include <stdbool.h>
# include <stdint.h>
# include <string.h>
# include <stdio.h>

# include "list_config.h"

typedef struct	s_list
{
	struct s_list_node	*list;
	uint64_t			size;
}	t_list;

/*
	Sets all values of a t_list to 0
*/
int	list_new(t_list *vec);
/*
	Deletes and frees all data inside the t_list, it will all then be set to 0
*/
int	list_delete(t_list *vec, bool free_data);

/*
	Adds a new node to the back of the list containing data
*/
int	list_add_back(t_list *vec, LIST_TYPE data);
/*
	Adds a new node to the front of the list containing data	
*/
int	list_add_front(t_list *vec, LIST_TYPE data);

/*
	Deletes the first occurence of a node containing data, comparison is done using the cmp_func function
*/
int	list_delete_node(t_list *vec, LIST_TYPE data, bool (*cmp_func)(LIST_TYPE, LIST_TYPE), bool free_data);

LIST_TYPE	list_last(t_list *vec);
void		list_for_each(t_list *vec, void (*func)(LIST_TYPE));
void		list_for_each_param(t_list *vec, void (*func)(LIST_TYPE, void *), void *param);
bool		list_has(t_list *vec, LIST_TYPE data, bool (*cmp_func)(LIST_TYPE, LIST_TYPE));
LIST_TYPE	list_find(t_list *vec, LIST_TYPE data, bool (*cmp_func)(LIST_TYPE, LIST_TYPE));
LIST_TYPE	*list_to_array(t_list *vec);

#endif
