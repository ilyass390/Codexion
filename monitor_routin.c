#include "codexion.h"

void	log_burnout(t_table *table, t_coder *coder, long now)
{
	pthread_mutex_lock(&table->print_mutex);
	printf("%lld %d burned out\n", now - table->start_ms, coder->id);
	pthread_mutex_unlock(&table->print_mutex);
}

int	check_burnout(t_table *table, long long now, t_coder **dead)
{
	int	i;

	i = 0;
	while (i < table->args.nb_coders)
	{
		if (now - table->coders[i].last_compile_ms
			>= table->args.time_to_burnout)
		{
			*dead = &table->coders[i];
			table->stop = 1;
			pthread_cond_broadcast(&table->sched_cond);
			return (1);
		}
		i++;
	}
	return (0);
}

int	check_success(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->args.nb_coders)
	{
		if (table->coders[i].compile_count
			< table->args.number_of_compiles_required)
			return (0);
		i++;
	}
	return (1);
}

void	*monitor_routine(void *arg)
{
	t_table	*table;
	t_coder	*dead;
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
		dead = NULL;
		if (check_burnout(table, now, &dead))
		{
			pthread_mutex_unlock(&table->sched_lock);
			log_burnout(table, dead, now);
			return (NULL);
		}
		if (check_success(table))
		{
			table->stop = 1;
			pthread_cond_broadcast(&table->sched_cond);
			pthread_mutex_unlock(&table->sched_lock);
			return (NULL);
		}
		pthread_mutex_unlock(&table->sched_lock);
		usleep(1000);
	}
	return (NULL);
}
