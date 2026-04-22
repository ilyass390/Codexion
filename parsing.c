/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 11:44:10 by iamessag          #+#    #+#             */
/*   Updated: 2026/04/20 12:08:14 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

int	ft_parse_scheduler(char *str)
{
	if (strcmp(str, "fifo") == 0)
		return (FIFO);
	if (strcmp(str, "edf") == 0)
		return (EDF);
	return (-1);
}

static int	ft_validate_ranges(t_args *args)
{
	if (args->nb_coders < 1)
		return (ft_print_error("number_of_coders must be >= 1"), -1);
	return (0);
}

static int	ft_validate_numbers(char **argv)
{
	int		i;
	char	*c;

	c = "all arguments except scheduler must be valid positive integers";
	i = 1;
	while (i <= 7)
	{
		if (!ft_is_valid_number(argv[i]))
		{
			ft_print_error(c);
			return (-1);
		}
		i++;
	}
	return (0);
}

static int	ft_fill_args(char **argv, t_args *args)
{
	args->nb_coders = ft_atoi(argv[1]);
	if (args->nb_coders == -1)
		return (-1);
	args->time_to_burnout = ft_atoi(argv[2]);
	if (args->time_to_burnout == -1)
		return (-1);
	args->time_to_compile = ft_atoi(argv[3]);
	if (args->time_to_compile == -1)
		return (-1);
	args->time_to_debug = ft_atoi(argv[4]);
	if (args->time_to_debug == -1)
		return (-1);
	args->time_to_refactor = ft_atoi(argv[5]);
	if (args->time_to_refactor == -1)
		return (-1);
	args->nb_comp = ft_atoi(argv[6]);
	if (args->nb_comp == -1)
		return (-1);
	args->dongle_cooldown = ft_atoi(argv[7]);
	if (args->dongle_cooldown == -1)
		return (-1);
	return (0);
}

int	parse_args(int argc, char **argv, t_args *args)
{
	if (argc != 9)
		return (ft_print_error("Number of args should be 8"), -1);
	if (ft_validate_numbers(argv) == -1)
		return (-1);
	if (ft_fill_args(argv, args) == -1)
		return (-1);
	if (ft_validate_ranges(args) == -1)
		return (-1);
	args->scheduler = ft_parse_scheduler(argv[8]);
	if (args->scheduler == -1)
		return (ft_print_error("scheduler must be 'fifo' or 'edf'"), -1);
	return (0);
}
