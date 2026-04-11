#include <stdio.h>
#include <pthread.h>
#include "codexion.h"
int ready = 0;
int counter = 0;
pthread_mutex_t lock;
pthread_cond_t cond;


void *waiter(void *arg)
{
    pthread_mutex_lock(&lock);

    while (ready == 0)
    {
        pthread_cond_wait(&cond, &lock);
    }

    printf("I woke up! ready = %d\n", ready);

    pthread_mutex_unlock(&lock);
    return NULL;
}


void *signaler(void *arg)
{
    sleep(2);

    pthread_mutex_lock(&lock);

    ready = 1;

    pthread_cond_signal(&cond);

    pthread_mutex_unlock(&lock);
    return NULL;
}


int main()
{
    struct  timeval test;

    gettimeofday(&test, NULL);


    printf("%lu", test.tv_usec);

    return 0;
}
