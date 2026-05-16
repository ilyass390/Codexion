/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 21:03:31 by iamessag          #+#    #+#             */
/*   Updated: 2026/05/15 10:18:26 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	do_compile(t_coder *coder)
{
	t_table	*table;

	table = coder->table;
	if (!take_two_dongles(coder))
		return (0);
	update_compile_state(coder);
	log_state(coder, "is compiling");
	smart_sleep(table, table->args.time_to_compile);
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
	smart_sleep(table, table->args.time_to_debug);
	return (!get_stop(table));
}

int	do_refactor(t_coder *coder)
{
	t_table	*table;

	table = coder->table;
	if (get_stop(table))
		return (0);
	log_state(coder, "is refactoring");
	smart_sleep(table, table->args.time_to_refactor);
	return (!get_stop(table));
}

static void	wait_for_start(t_coder *coder)
{
	t_table	*table;

	table = coder->table;
	pthread_mutex_lock(&table->state_mutex);
	table->coders_ready++;
	pthread_cond_broadcast(&table->start_cond);
	while (!table->simulation_started && !table->stop)
		pthread_cond_wait(&table->start_cond, &table->state_mutex);
	pthread_mutex_unlock(&table->state_mutex);
}

void	*coder_routine(void *arg)
{
	t_coder	*coder;
	t_table	*table;

	coder = (t_coder *)arg;
	table = coder->table;
	wait_for_start(coder);
	if (get_stop(table))
		return (NULL);
	if (coder->id % 2 == 0)
		smart_sleep(table, table->args.time_to_compile
			+ table->args.dongle_cooldown);
	else if (coder->id == table->args.nb_coders
		&& table->args.nb_coders % 2 == 1)
		smart_sleep(table, 2 * (table->args.time_to_compile
				+ table->args.dongle_cooldown));
	while (!get_stop(table))
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
