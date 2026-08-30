#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
sem_t sem;
void *process(void *arg)
{
    int id=*(int*)arg;
    printf("Process %d is waiting for the semaphore\n",id);
    sem_wait(&sem);
    printf("process %d has entered the critical section\n",id);
    sleep(2);
    printf("process %d is leaving the critical section\n",id);
    sem_post(&sem);
}
int main()
{
    pthread_t t1,t2;
    int id1=1;
    int id2=2;
    sem_init(&sem,0,1);
    pthread_create(&t1,NULL,process,&id1);
    pthread_create(&t2,NULL,process,&id2);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    sem_destroy(&sem);
    return 0;
}