#include<stdio.h>
#include<pthread.h>
int count=0;
pthread_mutex_t lock;
void *process(void *arg)
{
    for(int i=0;i<5;i++)
    {
        pthread_mutex_lock(&lock);
        count++;
        printf("the value is %d\n",count);
        pthread_mutex_unlock(&lock);
    }
}
int main()
{
    pthread_t t1,t2;
    pthread_mutex_init(&lock,NULL);
    pthread_create(&t1,NULL,process,NULL);
    pthread_create(&t2,NULL,process,NULL);
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    pthread_mutex_destroy(&lock);
    printf("Final count is %d\n",count);
}