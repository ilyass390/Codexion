/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 11:43:38 by iamessag          #+#    #+#             */
/*   Updated: 2026/04/20 11:53:17 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	heap_init(t_heap *heap)
{
	heap->coders = malloc(sizeof(t_coder *) * HEAP_MAX);
	if (!heap->coders)
		return (-1);
	heap->coders_in = 0;
	return (0);
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
				heap_destroy(&table->dongles[i].heap);
			free(table->dongles);
			table->dongles = NULL;
			return (-1);
		}
		i++;
	}
	return (0);
}

int	init_table(t_table *table)
{
	memset(table, 0, sizeof(t_table));
	if (pthread_mutex_init(&table->sched_lock, NULL) != 0)
		return (-1);
	table->sched_lock_init = 1;
	if (pthread_cond_init(&table->sched_cond, NULL) != 0)
		return (-1);
	table->sched_cond_init = 1;
	if (init_dongles(table) == -1)
		return (-1);
	if (init_coders(table) == -1)
		return (-1);
	return (0);
}
