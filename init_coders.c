/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_coders.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 11:52:30 by iamessag          #+#    #+#             */
/*   Updated: 2026/04/20 11:53:06 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	assign_dongles(t_coder *coder, t_dongle *a, t_dongle *b)
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

static void	init_coder_dongles(t_coder *coder, t_table *table, int i)
{
	t_dongle	*left;
	t_dongle	*right;

	left = &table->dongles[i];
	if (i == table->args.nb_coders - 1)
		right = &table->dongles[0];
	else
		right = &table->dongles[i + 1];
	assign_dongles(coder, left, right);
}

static void	init_coder(t_coder *coder, t_table *table, int i)
{
	coder->id = i + 1;
	coder->table = table;
	init_coder_dongles(coder, table, i);
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
		init_coder(&table->coders[i], table, i);
		i++;
	}
	return (0);
}
