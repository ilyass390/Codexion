/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   take_two_dongles.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 11:44:17 by iamessag          #+#    #+#             */
/*   Updated: 2026/04/20 12:04:28 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	grant_dongles(t_coder *coder)
{
	int	scheduler;

	scheduler = coder->table->args.scheduler;
	coder->lowest->available_at = LONG_MAX;
	coder->highest->available_at = LONG_MAX;
	heap_pop(&coder->lowest->heap, scheduler);
	heap_pop(&coder->highest->heap, scheduler);
	coder->in_queue = 0;
	pthread_cond_broadcast(&coder->table->sched_cond);
}

void	cleanup_queues(t_coder *coder)
{
	int	scheduler;

	scheduler = coder->table->args.scheduler;
	heap_remove(&coder->lowest->heap, coder, scheduler);
	heap_remove(&coder->highest->heap, coder, scheduler);
	coder->in_queue = 0;
}

int	take_two_dongles(t_coder *coder)
{
	t_table	*table;

	table = coder->table;
	pthread_mutex_lock(&table->sched_lock);
	push_to_queues(coder);
	while (!table->stop)
	{
		if (both_ready(coder))
			break ;
		smart_wait(coder);
	}
	if (table->stop)
	{
		cleanup_queues(coder);
		pthread_mutex_unlock(&table->sched_lock);
		return (0);
	}
	grant_dongles(coder);
	pthread_mutex_unlock(&table->sched_lock);
	log_state(coder, "has taken a dongle");
	log_state(coder, "has taken a dongle");
	return (1);
}

void	release_two_dongles(t_coder *coder)
{
	t_table		*table;
	long long	now;

	table = coder->table;
	now = get_time_ms();
	pthread_mutex_lock(&table->sched_lock);
	coder->lowest->available_at = now + table->args.dongle_cooldown;
	coder->highest->available_at = now + table->args.dongle_cooldown;
	pthread_cond_broadcast(&table->sched_cond);
	pthread_mutex_unlock(&table->sched_lock);
}
