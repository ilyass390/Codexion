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
	if (str[i] == '+')
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
	if (str[i] == '-')
	{
		ft_print_error("invalid input");
		return (-1);
	}
	if (str[i] == '+')
		i++;
	result = 0;
	while (str[i])
	{
		result = result * 10 + (str[i] - '0');
		if (result > INT_MAX)
		{
			ft_print_error("invalid input");
			return (-1);
		}
		i++;
	}
	return ((int)result);
}

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
	if (args->number_of_coders < 1)
		return (ft_print_error("number_of_coders must be >= 1"), -1);
	return (0);
}

static int	ft_validate_numbers(char **argv)
{
	int	i;

	i = 1;
	while (i <= 7)
	{
		if (!ft_is_valid_number(argv[i]))
		{
			ft_print_error("all arguments except scheduler must be valid positive integers");
			return (-1);
		}
		i++;
	}
	return (0);
}

static int	ft_fill_args(char **argv, t_args *args)
{
	args->number_of_coders = ft_atoi(argv[1]);
	if (args->number_of_coders == -1)
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
	args->number_of_compiles_required = ft_atoi(argv[6]);
	if (args->number_of_compiles_required == -1)
		return (-1);
	args->dongle_cooldown = ft_atoi(argv[7]);
	if (args->dongle_cooldown == -1)
		return (-1);
	return (0);
}

int	parse_args(int argc, char **argv, t_args *args)
{
	if (argc != 9)
		return (ft_print_error("usage: ./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler"), -1);
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
