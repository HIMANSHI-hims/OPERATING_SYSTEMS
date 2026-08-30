#include<stdio.h>
#include<pthread.h>
int count=0;
pthread_mutex_t lock;
void *process(void *arg)
{
    int id=*(int*)arg;
    for(int i=0;i<5;i++)
    {
        pthread_mutex_lock(&lock);
        count++;
        printf("Process %d increments the count to %d\n",id,count);
        pthread_mutex_unlock(&lock);
    }
}
int main()
{
    pthread_t t1,t2;
    pthread_mutex_init(&lock,NULL);
    int id1=1;
    int id2=2;
    pthread_create(&t1,NULL,process,&id1);
    pthread_create(&t2,NULL,process,&id2);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    pthread_mutex_destroy(&lock);
    return 0;
}