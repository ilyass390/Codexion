/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_helper.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 12:04:59 by iamessag          #+#    #+#             */
/*   Updated: 2026/04/20 12:05:36 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	get_stop(t_table *table)
{
	int	stop;

	pthread_mutex_lock(&table->sched_lock);
	stop = table->stop;
	pthread_mutex_unlock(&table->sched_lock);
	return (stop);
}

void		smart_sleep(long long duration, t_table *table)
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

void	update_compile_state(t_coder *coder)
{
	t_table	*table;

	table = coder->table;
	pthread_mutex_lock(&table->sched_lock);
	coder->last_compile_ms = get_time_ms();
	coder->compile_count++;
	pthread_mutex_unlock(&table->sched_lock);
}

int	do_compile(t_coder *coder)
{
	t_table	*table;

	table = coder->table;
	if (!take_two_dongles(coder))
		return (0);
	update_compile_state(coder);
	log_state(coder, "is compiling");
	smart_sleep(table->args.time_to_compile, table);
	release_two_dongles(coder);
	if (get_stop(table))
		return (0);
	return (1);
}
