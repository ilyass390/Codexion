#include "codexion.h"

int	get_stop(t_table *table)
{
	int	stop;

	pthread_mutex_lock(&table->sched_lock);
	stop = table->stop;
	pthread_mutex_unlock(&table->sched_lock);
	return (stop);
}

void	smart_sleep(long long duration, t_table *table)
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

int	do_debug(t_coder *coder)
{
	t_table	*table;

	table = coder->table;
	if (get_stop(table))
		return (0);
	log_state(coder, "is debugging");
	smart_sleep(table->args.time_to_debug, table);
	return (!get_stop(table));
}

int	do_refactor(t_coder *coder)
{
	t_table	*table;

	table = coder->table;
	if (get_stop(table))
		return (0);
	log_state(coder, "is refactoring");
	smart_sleep(table->args.time_to_refactor, table);
	return (!get_stop(table));
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	if (coder->id % 2 == 0)
		usleep((coder->table->args.time_to_compile
				+ coder->table->args.dongle_cooldown) * 1000);
	else if (coder->id == coder->table->args.nb_coders
		&& coder->table->args.nb_coders % 2 == 1)
		usleep(2 * (coder->table->args.time_to_compile
				+ coder->table->args.dongle_cooldown) * 1000);
	while (!get_stop(coder->table))
	{
		if (!do_compile(coder))
			break ;
		if (!do_debug(coder))
			break ;
		if (!do_refactor(coder))
			break ;
	}
	return (NULL);
}
