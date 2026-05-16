/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 21:03:20 by iamessag          #+#    #+#             */
/*   Updated: 2026/05/14 21:31:58 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		if (table->dongles[i].mutex_init)
			pthread_mutex_destroy(&table->dongles[i].mutex);
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
	if (table->print_mutex_init)
		pthread_mutex_destroy(&table->print_mutex);
	if (table->state_mutex_init)
		pthread_mutex_destroy(&table->state_mutex);
	if (table->start_cond_init)
		pthread_cond_destroy(&table->start_cond);
}
