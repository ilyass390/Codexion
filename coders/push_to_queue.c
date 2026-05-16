/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   push_to_queue.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 21:04:57 by iamessag          #+#    #+#             */
/*   Updated: 2026/05/14 23:14:16 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	push_to_queues(t_coder *coder)
{
	t_table		*table;
	int			scheduler;
	long long	last_compile_ms;

	table = coder->table;
	if (coder->in_queue)
		return (0);
	scheduler = table->args.scheduler;
	coder->request_time = get_time_ms();
	pthread_mutex_lock(&table->state_mutex);
	last_compile_ms = coder->last_compile_ms;
	pthread_mutex_unlock(&table->state_mutex);
	coder->deadline = last_compile_ms + table->args.time_to_burnout;
	if (heap_push(&coder->lowest->heap, coder, scheduler))
		return (-1);
	if (coder->lowest != coder->highest)
	{
		if (heap_push(&coder->highest->heap, coder, scheduler) == -1)
		{
			heap_remove(&coder->lowest->heap, coder, scheduler);
			return (-1);
		}
	}
	coder->in_queue = 1;
	return (0);
}

int	both_ready(t_coder *coder)
{
	long long	now;

	if (coder->lowest == coder->highest)
		return (0);
	now = get_time_ms();
	return (heap_peek(&coder->lowest->heap) == coder
		&& heap_peek(&coder->highest->heap) == coder
		&& now >= coder->lowest->available_at
		&& now >= coder->highest->available_at);
}

void	grant_dongles(t_coder *coder)
{
	int	scheduler;

	scheduler = coder->table->args.scheduler;
	coder->lowest->available_at = LLONG_MAX;
	heap_pop(&coder->lowest->heap, scheduler);
	if (coder->lowest != coder->highest)
	{
		coder->highest->available_at = LLONG_MAX;
		heap_pop(&coder->highest->heap, scheduler);
	}
	coder->in_queue = 0;
}

void	cleanup_queues(t_coder *coder)
{
	int	scheduler;

	if (!coder->in_queue)
		return ;
	scheduler = coder->table->args.scheduler;
	heap_remove(&coder->lowest->heap, coder, scheduler);
	if (coder->lowest != coder->highest)
		heap_remove(&coder->highest->heap, coder, scheduler);
	coder->in_queue = 0;
}

void	release_two_dongles(t_coder *coder)
{
	t_table		*table;
	long long	now;

	table = coder->table;
	now = get_time_ms();
	lock_two_dongles(coder);
	coder->lowest->available_at = now + table->args.dongle_cooldown;
	if (coder->highest != coder->lowest)
		coder->highest->available_at = now + table->args.dongle_cooldown;
	unlock_two_dongles(coder);
}
