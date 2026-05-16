/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 21:04:36 by iamessag          #+#    #+#             */
/*   Updated: 2026/05/13 19:24:57 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	release_barrier(t_table *table)
{
	int	i;

	pthread_mutex_lock(&table->state_mutex);
	while (table->coders_ready < table->args.nb_coders && !table->stop)
		pthread_cond_wait(&table->start_cond, &table->state_mutex);
	table->start_ms = get_time_ms();
	i = 0;
	while (i < table->args.nb_coders)
	{
		table->coders[i].last_compile_ms = table->start_ms;
		table->coders[i].deadline = table->start_ms
			+ table->args.time_to_burnout;
		i++;
	}
	table->simulation_started = 1;
	pthread_cond_broadcast(&table->start_cond);
	pthread_mutex_unlock(&table->state_mutex);
}

int	start_coder_threads(t_table *table, int *created)
{
	int	i;

	i = 0;
	while (i < table->args.nb_coders)
	{
		if (pthread_create(&table->coders[i].thread, NULL,
				coder_routine, &table->coders[i]) != 0)
		{
			ft_print_error("coder thread creation failed");
			set_stop(table);
			pthread_cond_broadcast(&table->start_cond);
			return (-1);
		}
		(*created)++;
		i++;
	}
	return (0);
}

int	start_threads(t_table *table, int *created)
{
	if (pthread_create(&table->monitor, NULL, monitor_routine, table) != 0)
	{
		ft_print_error("monitor thread creation failed");
		set_stop(table);
		pthread_cond_broadcast(&table->start_cond);
		return (-1);
	}
	table->monitor_init = 1;
	if (start_coder_threads(table, created) == -1)
		return (-1);
	release_barrier(table);
	return (0);
}

void	join_threads(t_table *table, int created)
{
	int	i;

	i = 0;
	while (i < created)
	{
		pthread_join(table->coders[i].thread, NULL);
		i++;
	}
	if (table->monitor_init)
		pthread_join(table->monitor, NULL);
}

int	main(int argc, char **argv)
{
	t_table	table;
	t_args	args;
	int		created;

	created = 0;
	if (parse_args(argc, argv, &args) == -1)
		return (1);
	table.args = args;
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
