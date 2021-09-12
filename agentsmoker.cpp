//solution was derived from the psuedo code provided by http://www.cs.umd.edu/~hollings/cs412/s96/synch/smokers.html

#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <string>

#define SMOKERNUM 3
#define EMPTY 0
#define ROUNDS 10

int smokers,table = 0;
bool isFair = true;
int usedTabled[3] {0};
std::string ingrediant;
pthread_mutex_t tableMutex, newTurnMutex;
pthread_cond_t fullTableCond, emptyTableCond, smokeCond, newTurnCond;

//method that handles the logic of the agent
void* agent(void* arg)
{
    //resets table if all smokers have smoked atleast once
    for(int i = 0; i < ROUNDS; i++){
        if(usedTabled[0] != 0 && usedTabled[1] != 0 && usedTabled[2] != 0){
        for(int i = 0; i < SMOKERNUM;i++){
        usedTabled[i] = 0;
    }
    std::cout << "all smokers have smoked" <<std::endl;
    }
        pthread_mutex_lock(&tableMutex);
        std::cout << "table opened" <<std::endl;
        if(table == EMPTY)
        {
            //random table combination made
            table = rand()%3 + 1;
            //if table has already been used before, reroll table to "make it fair"
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
            //dictates whats on the table 
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
            std::cout << "table checks if filled" << std::endl;
            pthread_cond_wait(&emptyTableCond, &tableMutex);
            std::cout << "table checks if emptied" << std::endl;
        }
        pthread_mutex_unlock(&tableMutex);
    }
    return NULL;
}

std::string smokerIngrediant(int smoker){
    switch(smoker){
        case 1:
           ingrediant = "paper";
           break;
        case 2:
           ingrediant = "tobacco";
           break;
        case 3:
           ingrediant = "matches";
           break;
    }
    return ingrediant;
}

//logic to indicate whether a smoker should be smoking
void smoking(int smokerid){

    //waits until table is filled
    if(table == EMPTY){
        pthread_cond_wait(&fullTableCond, &tableMutex);
    }
    smokers++;
    std::cout << "Smoker " << smokerid << ": checks table" << std::endl; 

    if(table == smokerid){
        if(smokers < 3){
            pthread_cond_wait(&smokeCond, &newTurnMutex);
        }
        smokers = 0;
        table = EMPTY;
        std::cout << "Smoker " << smokerid << " is smoking using: " << smokerIngrediant(smokerid) <<std::endl; 
        pthread_cond_signal(&emptyTableCond);
        pthread_cond_broadcast(&newTurnCond); 
        sleep(1);
        std::cout << "agent indicates the ending of the round" << std::endl;
    } else{
        if(smokers == 3){
            pthread_cond_signal(&smokeCond);
        }
        pthread_cond_wait(&newTurnCond, &newTurnMutex);
    }

}

//logic that handles the smoker
void* smoker(void* smoker){
    int ingredient = *((int *)smoker);
    for(int i = 0; i < ROUNDS; i++){
        pthread_mutex_lock(&newTurnMutex);
        smoking(ingredient);
        pthread_mutex_unlock(&newTurnMutex);
        //std::cout << "smoker thread unlocking" << std::endl;
        }
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
    
    //creates threads and then gives them a function
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
        std::cout << "Smoker exit" << std::endl;
    }
    if(pthread_join(agentP, NULL) != 0){
        std::cerr << "failed to join Agent " << std::endl;
    }
    std::cout << "agent exit" << std::endl;

    pthread_cond_destroy(&fullTableCond);
    pthread_cond_destroy(&emptyTableCond);
    pthread_cond_destroy(&smokeCond);
    pthread_cond_destroy(&newTurnCond);
    pthread_mutex_destroy(&tableMutex);
    pthread_mutex_destroy(&newTurnMutex);


    return EXIT_SUCCESS;
}

