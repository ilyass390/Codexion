#ifndef CODEXION_H
# define CODEXION_H

# include <stdio.h>
# include <string.h>
# include <limits.h>
# include <sys/time.h>
# include <pthread.h>
# include <unistd.h>

# define FIFO 0
# define EDF  1

typedef struct s_sim t_sim;

typedef struct s_args
{
	int	number_of_coders;
	int	time_to_burnout;
	int	time_to_compile;
	int	time_to_debug;
	int	time_to_refactor;
	int	number_of_compiles_required;
	int	dongle_cooldown;
	int	scheduler;
}	t_args;

typedef struct s_waiter
{
	int				coder_id;
	struct timeval	request_time;
	long long		deadline;
}	t_waiter;

typedef struct s_queue
{
	t_waiter	*waiters;
	int			size;
	int			capacity;
	int			scheduler;
}	t_queue;

typedef struct s_dongle
{
	pthread_mutex_t	mutex;
	pthread_cond_t	cond;
	int				available;
	struct timeval	released_at;
	t_queue			queue;
	int				id;
}	t_dongle;

typedef struct s_coder
{
	int				id;
	pthread_t		thread;
	struct timeval	last_compile_start;
	int				compile_count;
	long long		deadline;
	t_dongle		*left_dongle;
	t_dongle		*right_dongle;
	t_sim			*sim;
}	t_coder;

struct s_sim
{
	t_args			args;
	t_coder			*coders;
	t_dongle		*dongles;
	pthread_mutex_t	log_mutex;
	int				stop;
	pthread_mutex_t	stop_mutex;
	struct timeval	start_time;
};

long long	get_time_ms(void);
long long	elapsed_ms(struct timeval start);
void		ft_usleep(long long ms);
void		print_log(t_sim *sim, int id, char *msg);

int		ft_is_digit(char c);
int		ft_is_valid_number(char *str);
int		ft_atoi(char *str);
int		ft_parse_scheduler(char *str);
void	ft_print_error(char *msg);
int		parse_args(int argc, char **argv, t_args *args);
int		init_sim(t_sim *sim, t_args args);
void	cleanup_sim(t_sim *sim);
#endif