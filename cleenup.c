#include "codexion.h"

void	cleanup_dongles(t_table *table)
{
	int	i;

	if (!table->dongles)
		return ;
	i = 0;
	while (i < table->args.nb_coders)
	{
		heap_destroy(&table->dongles[i].heap);
		i++;
	}
	free(table->dongles);
	table->dongles = NULL;
}

void	cleanup(t_table *table)
{
	if (table->coders)
	{
		free(table->coders);
		table->coders = NULL;
	}
	cleanup_dongles(table);
	if (table->sched_lock_init)
		pthread_mutex_destroy(&table->sched_lock);
	if (table->sched_cond_init)
		pthread_cond_destroy(&table->sched_cond);
	if (table->print_mutex_init)
		pthread_mutex_destroy(&table->print_mutex);
}
