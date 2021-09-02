#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <iostream>

#define NUMTHREAD 5

void ingredients(int placement){
    switch(placement){
        case 1:
            std::cout << "Tobacco and Matches" << std::endl;
            break;
        case 2:
            std::cout << "Paper and Matches" << std::endl;
            break;
        case 3:
            std::cout << "Paper and Tobacco" << std::endl;
    }
}

int main(){

return 0;
}

/*do{



}*/