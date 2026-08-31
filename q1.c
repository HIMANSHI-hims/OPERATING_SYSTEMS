/*1. One-Lane Bridge Synchronization
Problem Statement
A bridge can accommodate a maximum of 3 vehicles simultaneously. Vehicles from East and West directions cannot be on the bridge at the same time. One thread is created for each vehicle.*/
#include<stdio.h>
#include<pthread.h>
#include<unistd.h>
#define MAX_VEHICLES 3
pthread_mutex_t mutex;
pthread_cond_t east_cond,west_cond;
int direction=0;
int vehicle_count=0;
void *east_vehicle(void *arg)
{
    int id=*(int*)arg;
    pthread_mutex_lock(&mutex);
    while(direction==2||vehicle_count==MAX_VEHICLES)
    {
        pthread_cond_wait(&east_cond,&mutex);
    }
    direction=1;
    vehicle_count++;
    printf("East vehicle %d is on the bridge. Total vehicles on bridge: %d\n",id,vehicle_count);
    pthread_mutex_unlock(&mutex);
    sleep(1);
    pthread_mutex_lock(&mutex);
    vehicle_count--;
    printf("East vehicle %d has left the bridge. Total vehicles on bridge are : %d\n",id,vehicle_count);
    if(vehicle_count==0)
    {
        direction=0;
        pthread_cond_broadcast(&west_cond);
    }
    else
    {
        pthread_cond_signal(&east_cond);
    }
    pthread_mutex_unlock(&mutex);
}
void *west_vehicle(void *arg)
{
    int id=*(int*)arg;
    pthread_mutex_lock(&mutex);
    while(direction==1||vehicle_count==MAX_VEHICLES)
    {
        pthread_cond_wait(&west_cond,&mutex);
    }
    direction=2;
    vehicle_count++;
    printf("West vehicle %d is on the bridge. Total vehicles on bridge: %d\n",id,vehicle_count);
    pthread_mutex_unlock(&mutex);
    sleep(1);
    pthread_mutex_lock(&mutex);
    vehicle_count--;
    printf("West vehicle %d has left the bridge. Total vehicles on bridge are : %d\n",id,vehicle_count);
    if(vehicle_count==0)
    {
        direction=0;
        pthread_cond_broadcast(&east_cond);
    }
    else
    {
        pthread_cond_signal(&west_cond);
    }
    pthread_mutex_unlock(&mutex);
}
int main()
{
    pthread_t east_threads[5],west_threads[5];
    int east_id[5],west_id[5];
    pthread_mutex_init(&mutex,NULL);
    pthread_cond_init(&east_cond,NULL);
    pthread_cond_init(&west_cond,NULL);
    for(int i=0;i<5;i++)
    {
        east_id[i]=i+1;
        west_id[i]=i+1;
    }
    for(int i=0;i<5;i++)
    {
        pthread_create(&east_threads[i],NULL,east_vehicle,&east_id[i]);
        pthread_create(&west_threads[i],NULL,west_vehicle,&west_id[i]);
    }
    for(int i=0;i<5;i++)
    {
        pthread_join(east_threads[i],NULL);
        pthread_join(west_threads[i],NULL);
    }
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&east_cond);
    pthread_cond_destroy(&west_cond);
    return 0;
}