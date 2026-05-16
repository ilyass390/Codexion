/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/10 21:05:14 by iamessag          #+#    #+#             */
/*   Updated: 2026/05/15 18:23:41 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ft_print_error(char *msg)
{
	fprintf(stderr, "Error: %s\n", msg);
}

int	ft_is_digit(char c)
{
	return (c >= '0' && c <= '9');
}

int	ft_is_valid_number(char *str)
{
	int	i;

	if (!str || !*str)
		return (0);
	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
		i++;
	if (!str[i])
		return (0);
	while (str[i])
	{
		if (!ft_is_digit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

int	ft_atoi(char *str)
{
	long	result;
	int		i;

	i = 0;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+')
		i++;
	result = 0;
	while (str[i])
	{
		result = result * 10 + (str[i] - '0');
		if (result > INT_MAX)
		{
			ft_print_error("number too large");
			return (-1);
		}
		i++;
	}
	return ((int)result);
}

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((long long)tv.tv_sec * 1000 + (tv.tv_usec / 1000));
}
