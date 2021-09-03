//solution was derived from the psuedo code provided by http://www.cs.umd.edu/~hollings/cs412/s96/synch/smokers.html

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>

#define SMOKERS 3
#define EMPTY 0

int table = 0;
pthread_mutex_t tableMutex;
pthread_cond_t fullTable;
pthread_cond_t emptyTable;

void* agent(void* arg)
{
    for(int i = 0; i < 3; i++){
        pthread_mutex_lock(&tableMutex);
        if(table == EMPTY)
        {
            //Fills table and waits for empty signal
            table = rand()%3 + 1;
            switch(table){
              case 1:
            std::cout << "Tobacco and Matches" << std::endl;
            break;
              case 2:
            std::cout << "Paper and Matches" << std::endl;
            break;
              case 3:
            std::cout << "Paper and Tobacco" << std::endl;
    }
            pthread_cond_signal(&fullTable);
            pthread_cond_wait(&emptyTable, &tableMutex);
        }
        pthread_mutex_unlock(&tableMutex);
    }
    return NULL;
}

int main(){
     pthread_t agent, smokers[SMOKERS];

return 0;
}

