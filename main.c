/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 11:43:51 by iamessag          #+#    #+#             */
/*   Updated: 2026/04/20 23:10:44 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	start_threads(t_table *table, int *created)
{
	int	i;

	if (pthread_create(&table->monitor, NULL, monitor_routine, table) != 0)
	{
		ft_print_error("monitor thread creation failed");
		return (-1);
	}
	table->monitor_init = 1;
	i = 0;
	while (i < table->args.nb_coders)
	{
		if (pthread_create(&table->coders[i].thread, NULL, coder_routine,
				&table->coders[i]) != 0)
		{
			ft_print_error("coder thread creation failed");
			pthread_mutex_lock(&table->sched_lock);
			table->stop = 1;
			pthread_cond_broadcast(&table->sched_cond);
			pthread_mutex_unlock(&table->sched_lock);
			return (-1);
		}
		(*created)++;
		i++;
	}
	return (0);
}

void	join_threads(t_table *table, int created)
{
	int	i;

	if (table->monitor_init)
		pthread_join(table->monitor, NULL);
	i = 0;
	while (i < created)
	{
		pthread_join(table->coders[i].thread, NULL);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_table	table;
	t_args	args;
	int		created;

	if (parse_args(argc, argv, &args) == -1)
		return (1);
	table.args = args;
	created = 0;
	if (init_table(&table) == -1)
	{
		ft_print_error("init failed");
		cleanup(&table);
		return (1);
	}
	if (start_threads(&table, &created) == -1)
	{
		join_threads(&table, created);
		cleanup(&table);
		return (1);
	}
	join_threads(&table, created);
	cleanup(&table);
	return (0);
}
