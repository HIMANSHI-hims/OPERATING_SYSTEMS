#include<stdio.h>
#include<pthread.h>
#include<semaphore.h>
#include<unistd.h>
sem_t sem;
int count=0;
void *process(void *arg)
{   int id=*(int*)arg;
    for(int i=0;i<5;i++)
    {
        sem_wait(&sem);
        count++;
        sem_post(&sem);
        printf("Process %d incremented to count %d\n",id,count);
    }
}
int main()
{
    pthread_t t1,t2,t3;
    int id1=1;
    int id2=2;
    int id3=3;
    sem_init(&sem,0,1);
    pthread_create(&t1,NULL,process,&id1);
    pthread_create(&t2,NULL,process,&id2);  
    pthread_create(&t3,NULL,process,&id3);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    pthread_join(t3,NULL);
    printf("Final count for all the processes is %d\n",count);
    sem_destroy(&sem);
    return 0;
}