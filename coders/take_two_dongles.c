/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_two_dongles.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 21:05:07 by iamessag          #+#    #+#             */
/*   Updated: 2026/05/13 21:40:57 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	try_take_dongles(t_coder *coder)
{
	lock_two_dongles(coder);
	if (push_to_queues(coder) == -1)
	{
		cleanup_queues(coder);
		unlock_two_dongles(coder);
		return (-1);
	}
	if (both_ready(coder))
	{
		grant_dongles(coder);
		unlock_two_dongles(coder);
		log_state(coder, "has taken a dongle");
		log_state(coder, "has taken a dongle");
		return (1);
	}
	unlock_two_dongles(coder);
	return (0);
}

void	leave_queues(t_coder *coder)
{
	lock_two_dongles(coder);
	cleanup_queues(coder);
	unlock_two_dongles(coder);
}

int	take_two_dongles(t_coder *coder)
{
	int	result;

	while (!get_stop(coder->table))
	{
		result = try_take_dongles(coder);
		if (result == 1)
			return (1);
		if (result == -1)
		{
			leave_queues(coder);
			return (0);
		}
		smart_sleep(coder->table, 1);
	}
	leave_queues(coder);
	return (0);
}
