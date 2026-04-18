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
		coder->deadline = coder->last_compile_ms
			+ table->args.time_to_burnout;
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
			pthread_cond_timedwait(&table->sched_cond,
				&table->sched_lock, &ts);
		}
	}
	else
		pthread_cond_wait(&table->sched_cond, &table->sched_lock);
}

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
	t_table	*table;
	long long	now;

	table = coder->table;
	now = get_time_ms();
	pthread_mutex_lock(&table->sched_lock);
	coder->lowest->available_at = now + table->args.dongle_cooldown;
	coder->highest->available_at = now + table->args.dongle_cooldown;
	pthread_cond_broadcast(&table->sched_cond);
	pthread_mutex_unlock(&table->sched_lock);
}
