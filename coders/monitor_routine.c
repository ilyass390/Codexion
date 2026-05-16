/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_routine.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 21:04:40 by iamessag          #+#    #+#             */
/*   Updated: 2026/05/13 19:27:30 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	check_burnout(t_table *table, long long now, t_coder **dead)
{
	int	i;

	pthread_mutex_lock(&table->state_mutex);
	i = 0;
	while (i < table->args.nb_coders)
	{
		if (now - table->coders[i].last_compile_ms
			>= table->args.time_to_burnout)
		{
			*dead = &table->coders[i];
			table->stop = 1;
			pthread_mutex_unlock(&table->state_mutex);
			return (1);
		}
		i++;
	}
	pthread_mutex_unlock(&table->state_mutex);
	return (0);
}

int	check_success(t_table *table)
{
	int	i;

	pthread_mutex_lock(&table->state_mutex);
	i = 0;
	while (i < table->args.nb_coders)
	{
		if (table->coders[i].compile_count < table->args.nb_comp)
		{
			pthread_mutex_unlock(&table->state_mutex);
			return (0);
		}
		i++;
	}
	table->stop = 1;
	pthread_mutex_unlock(&table->state_mutex);
	return (1);
}

static void	wait_for_start(t_table *table)
{
	pthread_mutex_lock(&table->state_mutex);
	while (!table->simulation_started && !table->stop)
		pthread_cond_wait(&table->start_cond, &table->state_mutex);
	pthread_mutex_unlock(&table->state_mutex);
}

void	*monitor_routine(void *arg)
{
	t_table		*table;
	t_coder		*dead;
	long long	now;

	table = (t_table *)arg;
	wait_for_start(table);
	while (!get_stop(table))
	{
		dead = NULL;
		now = get_time_ms();
		if (check_burnout(table, now, &dead))
		{
			log_burnout(table, dead);
			return (NULL);
		}
		if (check_success(table))
			return (NULL);
		usleep(300);
	}
	return (NULL);
}
