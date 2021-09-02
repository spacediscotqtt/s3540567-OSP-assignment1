#include<pthread.h>
#include<semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h> 

#define MAXTIME 10
#define BUCKET 10 
#define NUMTHREAD 5

sem_t empty;
sem_t full;
int in = 0;
int out = 0;
int buffer[BUCKET];
pthread_mutex_t mutex;
struct timeval startTime, endTime;
double elapsedTime = 0;

void *producer(void *pno)
{   
    int item, i;
    while(elapsedTime < 10) {
        item = rand() % 100; // Produce an random item
        sem_wait(&empty);
        pthread_mutex_lock(&mutex); 
        buffer[in] = item;
        printf("Producer %d: Insert Item %d at %d\n", *((int *)pno),buffer[in],in);
        in = (in+1)%BUCKET;
        pthread_mutex_unlock(&mutex); 
        sem_post(&full);
        usleep(100);
        gettimeofday(&endTime, NULL);
        elapsedTime = endTime.tv_sec - startTime.tv_sec;
    }
	return NULL;
}

void *consumer(void *cno)
{   
    int i;
	while(elapsedTime < 10) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex); 
        int item = buffer[out];
        printf("Consumer %d: Remove Item %d from %d\n",*((int *)cno),item, out);
        out = (out+1)%BUCKET;
        pthread_mutex_unlock(&mutex); //
        sem_post(&empty);
        gettimeofday(&endTime, NULL);
        elapsedTime = endTime.tv_sec - startTime.tv_sec;
    }
	return NULL;
}

int main()
{   
    pthread_t pro[5],con[5];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty,0,BUCKET);
    sem_init(&full,0,0);

    int a[5] = {1,2,3,4,5}; 
    gettimeofday(&startTime, NULL);

	int i;
    for(i = 0; i < 5; i++) {
        pthread_create(&pro[i], NULL, &producer, (void *)&a[i]);
    }
    for(i = 0; i < 5; i++) {
        pthread_create(&con[i], NULL, &consumer, (void *)&a[i]);
    }

    for(i = 0; i < 5; i++) {
        pthread_join(pro[i], NULL);
    }
    for(i = 0; i < 5; i++) {
        pthread_join(con[i], NULL);
    }

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}