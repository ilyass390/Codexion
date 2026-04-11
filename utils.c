#include "codexion.h"

long long	get_time_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

long long	elapsed_ms(struct timeval start)
{
	return (get_time_ms() - ((start.tv_sec * 1000) + (start.tv_usec / 1000)));
}

void	ft_usleep(long long ms)
{
	long long	start;

	start = get_time_ms();
	while (get_time_ms() - start < ms)
		usleep(500);
}

void	print_log(t_sim *sim, int id, char *msg)
{
	pthread_mutex_lock(&sim->log_mutex);
	printf("%lld %d %s\n", elapsed_ms(sim->start_time), id, msg);
	pthread_mutex_unlock(&sim->log_mutex);
}