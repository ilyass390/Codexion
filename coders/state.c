/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   state.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 21:05:03 by iamessag          #+#    #+#             */
/*   Updated: 2026/05/10 21:05:04 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	get_stop(t_table *table)
{
	int	stop;

	pthread_mutex_lock(&table->state_mutex);
	stop = table->stop;
	pthread_mutex_unlock(&table->state_mutex);
	return (stop);
}

void	set_stop(t_table *table)
{
	pthread_mutex_lock(&table->state_mutex);
	table->stop = 1;
	pthread_mutex_unlock(&table->state_mutex);
}

void	update_compile_state(t_coder *coder)
{
	t_table	*table;

	table = coder->table;
	pthread_mutex_lock(&table->state_mutex);
	coder->last_compile_ms = get_time_ms();
	coder->compile_count++;
	pthread_mutex_unlock(&table->state_mutex);
}

void	smart_sleep(t_table *table, long long duration)
{
	long long	start;

	start = get_time_ms();
	while (!get_stop(table))
	{
		if (get_time_ms() - start >= duration)
			break ;
		usleep(500);
	}
}
