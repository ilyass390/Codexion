/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routin.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 11:43:58 by iamessag          #+#    #+#             */
/*   Updated: 2026/04/20 22:46:32 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static int	handle_burnout(t_table *table, long long now)
{
	t_coder	*dead;

	dead = NULL;
	if (check_burnout(table, now, &dead))
	{
		pthread_mutex_unlock(&table->sched_lock);
		log_burnout(table, dead, now);
		return (1);
	}
	return (0);
}

static int	handle_success(t_table *table)
{
	if (check_success(table))
	{
		table->stop = 1;
		pthread_cond_broadcast(&table->sched_cond);
		pthread_mutex_unlock(&table->sched_lock);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_table		*table;
	long long	now;

	table = (t_table *)arg;
	while (1)
	{
		pthread_mutex_lock(&table->sched_lock);
		if (table->stop)
		{
			pthread_mutex_unlock(&table->sched_lock);
			return (NULL);
		}
		now = get_time_ms();
		if (handle_burnout(table, now))
			return (NULL);
		if (handle_success(table))
			return (NULL);
		pthread_mutex_unlock(&table->sched_lock);
		usleep(300);
	}
	return (NULL);
}
