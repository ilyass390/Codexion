#include "codexion.h"

int	heap_init(t_heap *heap)
{
	heap->coders = malloc(sizeof(t_coder *) * HEAP_MAX);
	if (!heap->coders)
		return (-1);
	heap->coders_in = 0;
	return (0);
}

void	assign_dongles(t_coder *coder, t_dongle *a, t_dongle *b)
{
	if (a->id < b->id)
	{
		coder->lowest = a;
		coder->highest = b;
	}
	else
	{
		coder->lowest = b;
		coder->highest = a;
	}
}

int	init_dongles(t_table *table)
{
	int	i;

	table->dongles = malloc(sizeof(t_dongle) * table->args.nb_coders);
	if (!table->dongles)
		return (-1);
	i = 0;
	while (i < table->args.nb_coders)
	{
		table->dongles[i].id = i;
		table->dongles[i].available_at = table->start_ms;
		if (heap_init(&table->dongles[i].heap) == -1)
		{
			while (--i >= 0)
				free(table->dongles[i].heap.coders);
			free(table->dongles);
			table->dongles = NULL;
			return (-1);
		}
		i++;
	}
	return (0);
}

int	init_coders(t_table *table)
{
	t_dongle	*left;
	t_dongle	*right;
	int			i;

	table->coders = malloc(sizeof(t_coder) * table->args.nb_coders);
	if (!table->coders)
		return (-1);
	i = 0;
	while (i < table->args.nb_coders)
	{
		table->coders[i].id = i + 1;
		table->coders[i].last_compile_ms = table->start_ms;
		table->coders[i].request_time = 0;
		table->coders[i].deadline = table->start_ms
			+ table->args.time_to_burnout;
		table->coders[i].compile_count = 0;
		table->coders[i].in_queue = 0;
		table->coders[i].table = table;
		left = &table->dongles[i];
		if (i == table->args.nb_coders - 1)
			right = &table->dongles[0];
		else
			right = &table->dongles[i + 1];
		assign_dongles(&table->coders[i], left, right);
		i++;
	}
	return (0);
}

int	init_table(t_table *table)
{
	table->start_ms = get_time_ms();
	table->stop = 0;
	table->dongles = NULL;
	table->coders = NULL;
	table->sched_lock_init = 0;
	table->sched_cond_init = 0;
	table->print_mutex_init = 0;
	if (pthread_mutex_init(&table->sched_lock, NULL) != 0)
		return (-1);
	table->sched_lock_init = 1;
	if (pthread_cond_init(&table->sched_cond, NULL) != 0)
		return (-1);
	table->sched_cond_init = 1;
	if (pthread_mutex_init(&table->print_mutex, NULL) != 0)
		return (-1);
	table->print_mutex_init = 1;
	if (init_dongles(table) == -1)
		return (-1);
	if (init_coders(table) == -1)
		return (-1);
	return (0);
}
