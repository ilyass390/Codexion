/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_coder.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/13 19:23:04 by iamessag          #+#    #+#             */
/*   Updated: 2026/05/13 19:23:06 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

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

void	init_coder_dongles(t_table *table, int i)
{
	t_dongle	*left;
	t_dongle	*right;

	left = &table->dongles[i];
	if (i == table->args.nb_coders - 1)
		right = &table->dongles[0];
	else
		right = &table->dongles[i + 1];
	assign_dongles(&table->coders[i], left, right);
}

void	init_coder(t_table *table, int i)
{
	table->coders[i].id = i + 1;
	table->coders[i].compile_count = 0;
	table->coders[i].last_compile_ms = 0;
	table->coders[i].request_time = 0;
	table->coders[i].deadline = 0;
	table->coders[i].in_queue = 0;
	table->coders[i].table = table;
	init_coder_dongles(table, i);
}

int	init_coders(t_table *table)
{
	int	i;

	table->coders = malloc(sizeof(t_coder) * table->args.nb_coders);
	if (!table->coders)
		return (-1);
	i = 0;
	while (i < table->args.nb_coders)
	{
		init_coder(table, i);
		i++;
	}
	return (0);
}
