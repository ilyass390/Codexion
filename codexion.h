/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iamessag <iamessag@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/20 11:42:49 by iamessag          #+#    #+#             */
/*   Updated: 2026/04/20 22:40:52 by iamessag         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <limits.h>
# include <pthread.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/time.h>
# include <unistd.h>

# define FIFO 0
# define EDF 1

# define HEAP_MAX 2	

typedef struct s_args
{
	int					nb_coders;
	int					time_to_burnout;
	int					time_to_compile;
	int					time_to_debug;
	int					time_to_refactor;
	int					nb_comp;
	int					dongle_cooldown;
	int					scheduler;
}						t_args;

typedef struct s_coder	t_coder;
typedef struct s_table	t_table;

typedef struct s_heap
{
	t_coder				**coders;
	int					coders_in;
}						t_heap;

typedef struct s_dongle
{
	int					id;
	long long			available_at;
	t_heap				heap;
}						t_dongle;

struct					s_coder
{
	int					id;
	t_dongle			*lowest;
	t_dongle			*highest;
	long long			last_compile_ms;
	long long			request_time;
	long long			deadline;
	int					compile_count;
	int					in_queue;
	t_table				*table;
	pthread_t			thread;
};

struct s_table
{
	t_args			args;
	t_coder			*coders;
	t_dongle		*dongles;
	long long		start_ms;
	int				stop;
	pthread_mutex_t	sched_lock;
	pthread_cond_t	sched_cond;
	int				sched_lock_init;
	int				sched_cond_init;
	int				monitor_init;
	pthread_t		monitor;
	int    coders_ready;
};

long long				get_time_ms(void);
void					ft_usleep(long long ms);

int						ft_is_digit(char c);
int						ft_is_valid_number(char *str);
int						ft_atoi(char *str);
int						ft_parse_scheduler(char *str);
void					ft_print_error(char *msg);
int						parse_args(int argc, char **argv, t_args *args);

int						heap_init(t_heap *heap);
void					assign_dongles(t_coder *coder, t_dongle *a,
							t_dongle *b);
int						init_dongles(t_table *table);
int						init_coders(t_table *table);
int						init_table(t_table *table);

int						heap_compare(t_coder *a, t_coder *b, int scheduler);
void					heap_swap(t_heap *heap, int a, int b);
void					bubble_up(t_heap *heap, int i, int scheduler);
void					bubble_down(t_heap *heap, int i, int scheduler);
int						heap_push(t_heap *heap, t_coder *coder, int scheduler);
t_coder					*heap_peek(t_heap *heap);
void					heap_pop(t_heap *heap, int scheduler);
void					heap_destroy(t_heap *heap);
void					heap_remove(t_heap *heap, t_coder *coder,
							int scheduler);

int						get_stop(t_table *table);

void					log_state(t_coder *coder, char *msg);
void					smart_sleep(long long duration, t_table *table);
void					update_compile_state(t_coder *coder);
int						do_compile(t_coder *coder);
int						do_debug(t_coder *coder);
int						do_refactor(t_coder *coder);
void					*coder_routine(void *arg);

void					ms_to_timespec(struct timespec *ts, long long ms);
long long				long_max(long long a, long long b);
void					push_to_queues(t_coder *coder);
int						both_ready(t_coder *coder);
void					smart_wait(t_coder *coder);
void					grant_dongles(t_coder *coder);
void					cleanup_queues(t_coder *coder);
int						take_two_dongles(t_coder *coder);
void					release_two_dongles(t_coder *coder);

void					log_burnout(t_table *table, t_coder *coder,
							long long now);
int						check_burnout(t_table *table, long long now,
							t_coder **dead);
int						check_success(t_table *table);
void					*monitor_routine(void *arg);

void					cleanup_dongles(t_table *table);
void					cleanup(t_table *table);

void					join_threads(t_table *table, int created);
int						start_threads(t_table *table, int *created);

#endif
