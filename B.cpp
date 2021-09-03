//solution was derived from the psuedo code provided by http://www.cs.umd.edu/~hollings/cs412/s96/synch/smokers.html

#include <time.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>

#define SMOKERNUM 3
#define EMPTY 0

int smokers = 0;
int table = 0;
pthread_mutex_t tableMutex, newTurnMutex;
pthread_cond_t fullTableCond;
pthread_cond_t emptyTableCond;
pthread_cond_t smokeCond;
pthread_cond_t newTurnCond;


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
                    std::cout << "Tobacco and Matches placed by agent" << std::endl; //Paper
                    break;
                case 2:
                    std::cout << "Paper and Matches placed by agent" << std::endl; //Tobacco
                    break;
                case 3:
                    std::cout << "Paper and Tobacco placed by agent" << std::endl; //Matches
                    break;
        }
            pthread_cond_signal(&fullTableCond);
            pthread_cond_wait(&emptyTableCond, &tableMutex);
        }
        pthread_mutex_unlock(&tableMutex);
    }
    return NULL;
}

void smoking(int smokerid){

    if(table == EMPTY){
        pthread_cond_wait(&fullTableCond, &tableMutex);
    }
    smokers++;
    printf("Smoker %d: checks table", smokerid);

    if(table == smokerid){
        if(smokers < 3){
            pthread_cond_wait(&smokeCond, &newTurnMutex);
        }
        smokers = 0;
        table = EMPTY;
        pthread_cond_signal(&emptyTableCond);
        printf("smoker: %d, is smoking", smokerid);
        //pthread_cond_broadcast

    }

void smoker(void* smoker){
// do forever {
//       P( smoker_tobacco );  // Sleep right away
//       P( lock );
//       Pick up match
//       Pick up paper
//        V( agent );
//        V( lock );
//        Smoke (but don't inhale)
//     }
return NULL;
}

int main(){
     pthread_t agent, smokers[SMOKERNUM];

return 0;
}

