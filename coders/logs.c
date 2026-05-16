/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logs.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 21:04:26 by iamessag          #+#    #+#             */
/*   Updated: 2026/05/10 21:04:29 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	log_state(t_coder *coder, char *msg)
{
	t_table		*table;
	long long	now;

	table = coder->table;
	pthread_mutex_lock(&table->state_mutex);
	if (table->stop)
	{
		pthread_mutex_unlock(&table->state_mutex);
		return ;
	}
	pthread_mutex_lock(&table->print_mutex);
	now = get_time_ms() - table->start_ms;
	printf("%lld %d %s\n", now, coder->id, msg);
	pthread_mutex_unlock(&table->print_mutex);
	pthread_mutex_unlock(&table->state_mutex);
}

void	log_burnout(t_table *table, t_coder *coder)
{
	long long	now;

	pthread_mutex_lock(&table->print_mutex);
	now = get_time_ms() - table->start_ms;
	printf("%lld %d burned out\n", now, coder->id);
	pthread_mutex_unlock(&table->print_mutex);
}
