/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   takes_two_dongles_helpers.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 12:04:02 by iamessag          #+#    #+#             */
/*   Updated: 2026/04/20 12:04:32 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ms_to_timespec(struct timespec *ts, long long ms)
{
	ts->tv_sec = ms / 1000;
	ts->tv_nsec = (ms % 1000) * 1000000L;
}

long long	long_max(long long a, long long b)
{
	if (a > b)
		return (a);
	return (b);
}

void	push_to_queues(t_coder *coder)
{
	t_table	*table;

	table = coder->table;
	if (!coder->in_queue)
	{
		coder->request_time = get_time_ms();
		coder->deadline = coder->last_compile_ms + table->args.time_to_burnout;
		heap_push(&coder->lowest->heap, coder, table->args.scheduler);
		heap_push(&coder->highest->heap, coder, table->args.scheduler);
		coder->in_queue = 1;
	}
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

void	smart_wait(t_coder *coder)
{
	t_table			*table;
	struct timespec	ts;
	long long		wake_at;
	long long		now;

	table = coder->table;
	if (heap_peek(&coder->lowest->heap) == coder
		&& heap_peek(&coder->highest->heap) == coder)
	{
		now = get_time_ms();
		wake_at = long_max(coder->lowest->available_at,
				coder->highest->available_at);
		if (wake_at <= now)
			pthread_cond_wait(&table->sched_cond, &table->sched_lock);
		else
		{
			ms_to_timespec(&ts, wake_at);
			pthread_cond_timedwait(&table->sched_cond, &table->sched_lock, &ts);
		}
	}
	else
		pthread_cond_wait(&table->sched_cond, &table->sched_lock);
}
