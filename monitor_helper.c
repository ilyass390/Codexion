/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_helper.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 12:09:38 by iamessag          #+#    #+#             */
/*   Updated: 2026/04/20 12:10:13 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_burnout(t_table *table, t_coder *coder, long long now)
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
		if (now
			- table->coders[i].last_compile_ms >= table->args.time_to_burnout)
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
		if (table->coders[i].compile_count < table->args.nb_comp)
			return (0);
		i++;
	}
	return (1);
}
