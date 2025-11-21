/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list_config.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mbatty <mbatty@student.42angouleme.fr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/21 08:28:22 by mbatty            #+#    #+#             */
/*   Updated: 2025/11/21 10:06:02 by mbatty           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIST_CONFIG_H
# define LIST_CONFIG_H

struct s_client;

// Type to use in the list
# define LIST_TYPE 	struct s_client *
// Check if we can call free on LIST_TYPE
# define LIST_CAN_FREE true

#endif
