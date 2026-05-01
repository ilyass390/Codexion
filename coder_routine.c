/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   coder_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 11:43:10 by iamessag          #+#    #+#             */
/*   Updated: 2026/04/20 12:05:31 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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
	pthread_mutex_lock(&coder->table->sched_lock);
    coder->table->coders_ready++;
    while (coder->table->coders_ready < coder->table->args.nb_coders
        && !coder->table->stop)
        pthread_cond_wait(&coder->table->sched_cond,
            &coder->table->sched_lock);
    pthread_mutex_unlock(&coder->table->sched_lock);
    if (get_stop(coder->table))
	{
        return (NULL);
	}
	if (coder->id % 2 == 0)
		smart_sleep(coder->table->args.time_to_compile
			+ coder->table->args.dongle_cooldown, coder->table);
	else if (coder->id == coder->table->args.nb_coders
		&& coder->table->args.nb_coders % 2 == 1)
		smart_sleep(2 * (coder->table->args.time_to_compile
				+ coder->table->args.dongle_cooldown), coder->table);
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
