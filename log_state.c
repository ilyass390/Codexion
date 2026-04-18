#include "codexion.h"

void	log_state(t_coder *coder, char *msg)
{
	t_table	*table;
	long	now;

	table = coder->table;
	pthread_mutex_lock(&table->sched_lock);
	if (table->stop)
	{
		pthread_mutex_unlock(&table->sched_lock);
		return ;
	}
	pthread_mutex_unlock(&table->sched_lock);
	pthread_mutex_lock(&table->print_mutex);
	if (!table->stop)
	{
		now = get_time_ms();
		printf("%lld %d %s\n", now - table->start_ms, coder->id, msg);
	}
	pthread_mutex_unlock(&table->print_mutex);
}
