//solution was derived from the psuedo code provided by http://www.cs.umd.edu/~hollings/cs412/s96/synch/smokers.html

#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>

#define SMOKERNUM 3
#define EMPTY 0

int smokers,table = 0;
bool isFair = true;
int tableComb[3] = {1,2,3};
int usedTabled[3] {0};
pthread_mutex_t tableMutex, newTurnMutex;
pthread_cond_t fullTableCond, emptyTableCond, smokeCond, newTurnCond;

void* agent(void* arg)
{
    for(int i = 0; i < 3; i++){
        if(usedTabled[0] != 0 && usedTabled[1] != 0 && usedTabled[2] != 0){
        for(int i = 0; i < SMOKERNUM;i++){
        usedTabled[i] = 0;
    }
    }
        pthread_mutex_lock(&tableMutex);
        if(table == EMPTY)
        {
            table = rand()%3 + 1;
            if(usedTabled[table - 1] == 0){
                usedTabled[table - 1] = table;                
            }else{
                isFair = false;
                while(isFair == false){
                    table = rand()%3 + 1;
                    if(usedTabled[table - 1] == 0){
                        usedTabled[table - 1] = table;
                        isFair = true;
                    }

                }
                isFair = true;
            }
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
    printf("Smoker %d: checks table\n", smokerid);

    if(table == smokerid){
        if(smokers < 3){
            pthread_cond_wait(&smokeCond, &newTurnMutex);
        }
        smokers = 0;
        table = EMPTY;
        pthread_cond_signal(&emptyTableCond);
        printf("smoker %d: is smoking\n", smokerid);
        pthread_cond_broadcast(&newTurnCond);
        sleep(2);
    } else{
        if(smokers == 3){
            pthread_cond_signal(&smokeCond);
        }
        pthread_cond_wait(&newTurnCond, &newTurnMutex);
    }

}

void* smoker(void* smoker){
    int ingredient = *((int *)smoker);
    for(int i = 0; i < 3; i++){
        pthread_mutex_lock(&newTurnMutex);
        smoking(ingredient);
        pthread_mutex_unlock(&newTurnMutex);
        }
    std::cout << "Smoker leaving" << std::endl;
    return NULL;
}


int main(){
    pthread_t agentP, smokers[SMOKERNUM];
    pthread_cond_init(&fullTableCond, NULL);
    pthread_cond_init(&emptyTableCond, NULL);
    pthread_cond_init(&smokeCond, NULL);
    pthread_cond_init(&newTurnCond, NULL);
    pthread_mutex_init(&tableMutex, NULL);
    pthread_mutex_init(&newTurnMutex, NULL);


    int label[3] = {1,2,3};
    
    //Creates threads and assigns them to function
    pthread_create(&agentP, NULL, &agent, NULL);
    for (int i = 0; i < 3; i++)
    {
        if (pthread_create(&smokers[i], NULL, &smoker, (void *)&label[i]) != 0)
        {
            std::cerr << "failed to create Smoker " << i << std::endl;
        }
    }

    //Waits for threads to finish and kills them
    for (int i = 0; i < 3; i++)
    {
        if (pthread_join(smokers[i], NULL) != 0)
        {
            std::cerr << "failed to join Smoker " << i << std::endl;
        }
    }
    if(pthread_join(agentP, NULL) != 0){
        std::cerr << "failed to join Agent " << std::endl;
    }

    pthread_cond_destroy(&fullTableCond);
    pthread_cond_destroy(&emptyTableCond);
    pthread_cond_destroy(&smokeCond);
    pthread_cond_destroy(&newTurnCond);
    pthread_mutex_destroy(&tableMutex);
    pthread_mutex_destroy(&newTurnMutex);


    return 0;
}

