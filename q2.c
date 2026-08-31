#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

#define RUNWAYS 2
#define AIRCRAFT 8

pthread_mutex_t mutex;
pthread_cond_t cond;

int available_runways = RUNWAYS;
int emergency_waiting = 0;


/* Aircraft function */

void *aircraft(void *arg)
{
    int id = *(int *)arg;

    /*
       Randomly decide whether aircraft
       is emergency or normal
    */

    int type = rand() % 2;

    pthread_mutex_lock(&mutex);

    if (type == 1)
    {
        /* Emergency aircraft */

        emergency_waiting++;

        printf("Emergency Aircraft %d requesting runway\n", id);

        /*
           Emergency aircraft waits only if
           no runway is available.
        */

        while (available_runways == 0)
        {
            pthread_cond_wait(&cond, &mutex);
        }

        emergency_waiting--;

        available_runways--;

        printf("Emergency Aircraft %d LANDING | Runways available = %d\n",
               id, available_runways);
    }
    else
    {
        /* Normal aircraft */

        printf("Normal Aircraft %d requesting runway\n", id);

        /*
           Normal aircraft must wait if:

           1. No runway is available
           OR
           2. Emergency aircraft are waiting
        */

        while (available_runways == 0 || emergency_waiting > 0)
        {
            pthread_cond_wait(&cond, &mutex);
        }

        available_runways--;

        printf("Normal Aircraft %d LANDING | Runways available = %d\n",
               id, available_runways);
    }

    pthread_mutex_unlock(&mutex);


    /*
       Simulate landing
    */

    sleep(2);


    /* Aircraft leaves runway */

    pthread_mutex_lock(&mutex);

    available_runways++;

    printf("Aircraft %d LEFT runway | Runways available = %d\n",
           id, available_runways);

    /*
       Wake waiting aircraft
    */

    pthread_cond_broadcast(&cond);

    pthread_mutex_unlock(&mutex);

    return NULL;
}


int main()
{
    pthread_t threads[AIRCRAFT];
    int id[AIRCRAFT];

    /* Initialize mutex and condition variable */

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&cond, NULL);

    /* Create aircraft threads */

    for (int i = 0; i < AIRCRAFT; i++)
    {
        id[i] = i + 1;

        pthread_create(&threads[i],
                       NULL,
                       aircraft,
                       &id[i]);

        /*
           Small delay so that aircraft requests
           don't all happen at exactly the same time.
        */

        usleep(100000);
    }

    /* Wait for all aircraft */

    for (int i = 0; i < AIRCRAFT; i++)
    {
        pthread_join(threads[i], NULL);
    }

    /* Destroy synchronization objects */

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);

    return 0;
}