//code used is referenced from: https://shivammitra.com/c/producer-consumer-problem-in-c/#
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h> 
#include <iostream>

//time in seconds
#define MAXTIME 10

//size of the bucket/buffer
#define BUCKETSIZE 10 

//
#define NUMTHREAD 5

sem_t emptySlots, fullSlots;
int inIndex = 0;
int outIndex = 0;
int bucket[BUCKETSIZE];
pthread_mutex_t threadLock;
struct timeval startTime, endTime;
double elapsedTime = 0;

void *producer(void *pno)
{   
    int item;
    //run the code for 10 seconds
    while(elapsedTime < MAXTIME) {
        // Produce an random item(int between 1-100 for simplicity)
        item = rand() % 100; 
        // waits/sleeps when there are no empty slots to be filled
        sem_wait(&emptySlots);
        // mutex locks in the current thread so other codes do not execute concurrently
        pthread_mutex_lock(&threadLock);
        std::cout << "mutex locked for producer thread: " << *((int *)pno) << std::endl;
        //item added to buffer
        bucket[inIndex] = item;
        std::cout << "Producer " << *((int *)pno) << " : Insert Item " <<bucket[inIndex]<< " at " <<  inIndex + 1 << std::endl;
        inIndex = (inIndex+1)%BUCKETSIZE;
        // mutex unlocks the code so other threads may begin to run
        pthread_mutex_unlock(&threadLock);
        std::cout << "mutex unlocked for producer thread: " << *((int *)pno) << std::endl;
        // indicates slot is full to indicate presence of an item
        sem_post(&fullSlots);
        usleep(100); 
        gettimeofday(&endTime, NULL);

        elapsedTime = endTime.tv_sec - startTime.tv_sec;
    }
	return NULL;
}

void *consumer(void *cno)
{   
	while(elapsedTime < MAXTIME) {
        // wait/sleep when nothing is in full slot
        sem_wait(&fullSlots); 
        pthread_mutex_lock(&threadLock);
        std::cout << "mutex locked for consumer thread: " << *((int *)cno) << std::endl; 
        int item = bucket[outIndex];
        std::cout << "Consumer " << *((int *)cno) << ": remove Item " << item << " at " <<  outIndex + 1 << std::endl;
        outIndex = (outIndex+1)%BUCKETSIZE;
        pthread_mutex_unlock(&threadLock); 
        std::cout << "mutex unlocked for consumer thread: " << *((int *)cno) << std::endl; 
        // indicates that there are empty slots that can be be taken up by an item.
        sem_post(&emptySlots);
        gettimeofday(&endTime, NULL);
        elapsedTime = endTime.tv_sec - startTime.tv_sec;
    }
	return NULL;
}

int main()
{   
    pthread_t pro[NUMTHREAD],con[NUMTHREAD];
    pthread_mutex_init(&threadLock, NULL);
    sem_init(&emptySlots,0,BUCKETSIZE);
    sem_init(&fullSlots,0,0);

    int a[5] = {1,2,3,4,5}; 
    gettimeofday(&startTime, NULL);

	int i;
    //creates threads and then gives them a function
    for(i = 0; i < 5; i++) {
        if(pthread_create(&pro[i], NULL, &producer, (void *)&a[i]) != 0)
        {
            std::cerr << "failed to create Producer " << i << std::endl;
        }
    }
    for(i = 0; i < 5; i++) {
        if(pthread_create(&con[i], NULL, &consumer, (void *)&a[i]) !=0)
        {
            std::cerr << "failed to create Consumer " << i << std::endl;
        }
    }

    //Waits for threads to finish and kills them
    for(i = 0; i < 5; i++) {
        if(pthread_join(pro[i], NULL) != 0)
        {
            std::cerr << "failed to join Producer " << i << std::endl;
        }
        std::cout << "producer exit" << std::endl; 
    }

    //Waits for threads to finish and kills them
    for(i = 0; i < 5; i++) {
        if(pthread_join(con[i], NULL) != 0)
        {
            std::cerr << "failed to join Consumer " << i << std::endl;
        }
        std::cout << "consumer exit" << std::endl; 
    }

    pthread_mutex_destroy(&threadLock);
    sem_destroy(&emptySlots);
    sem_destroy(&fullSlots);

    return EXIT_SUCCESS;
}