/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:22:54 by iamessag          #+#    #+#             */
/*   Updated: 2026/05/14 21:32:25 by iamessag         ###   ########.fr       */
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

static void	cleanup_dongles_partial(t_table *table, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		heap_destroy(&table->dongles[i].heap);
		if (table->dongles[i].mutex_init)
			pthread_mutex_destroy(&table->dongles[i].mutex);
		i++;
	}
	free(table->dongles);
	table->dongles = NULL;
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
		table->dongles[i].available_at = 0;
		table->dongles[i].mutex_init = 0;
		if (heap_init(&table->dongles[i].heap) == -1)
			return (cleanup_dongles_partial(table, i), -1);
		if (pthread_mutex_init(&table->dongles[i].mutex, NULL) != 0)
			return (cleanup_dongles_partial(table, i + 1), -1);
		table->dongles[i].mutex_init = 1;
		i++;
	}
	return (0);
}

int	init_table(t_table *table)
{
	table->start_ms = 0;
	table->stop = 0;
	table->coders_ready = 0;
	table->simulation_started = 0;
	table->dongles = NULL;
	table->coders = NULL;
	table->print_mutex_init = 0;
	table->state_mutex_init = 0;
	table->start_cond_init = 0;
	table->monitor_init = 0;
	if (pthread_mutex_init(&table->print_mutex, NULL) != 0)
		return (-1);
	table->print_mutex_init = 1;
	if (pthread_mutex_init(&table->state_mutex, NULL) != 0)
		return (-1);
	table->state_mutex_init = 1;
	if (pthread_cond_init(&table->start_cond, NULL) != 0)
		return (-1);
	table->start_cond_init = 1;
	if (init_dongles(table) == -1)
		return (-1);
	if (init_coders(table) == -1)
		return (-1);
	return (0);
}
