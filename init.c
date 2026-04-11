#include "codexion.h"

int	init_sim(t_sim *sim, t_args args)
{
    sim->args = args;
    gettimeofday(&sim->start_time, NULL);
    sim->stop = 0;
    pthread_mutex_init(&sim->log_mutex, NULL);
    pthread_mutex_init(&sim->stop_mutex, NULL);
    sim->dongles = malloc(sizeof(t_dongle) * args.number_of_coders);
    if (!sim->dongles)
        return (-1);
    sim->coders = malloc(sizeof(t_coder) * args.number_of_coders);
    if (!sim->coders)
        return (free(sim->dongles), -1);
    if (init_dongles(sim) == -1)
        return (-1);
    if (init_coders(sim) == -1)
        return (-1);
    return (0);
}

void	cleanup_sim(t_sim *sim)
{
    free(sim->coders);
    free(sim->dongles);
    pthread_mutex_destroy(&(sim->start_time));
    pthread_mutex_destroy(&(sim->stop_mutex));
}

int init_s-s_queue