#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

using namespace std;

pthread_cond_t entryCond,exitCond;
pthread_mutex_t mutex;
bool entryReady = false, exitReady = false, entryOpen = false, exitOpen = false;
bool carReady = false, carParked = false, carLeaving = false, carExited = false;
int delay = 0;
int numberOfCars = 0;

void* carFunc(void* arg);
void* entryFunc(void*);
void* exitFunc(void*);

struct carThreads
{
    int carNumber;
    int delay;
};


int main()
{
    cout << "How many cars would you like to create?" << endl;
    cin >> numberOfCars;
    if (numberOfCars < 1)
        numberOfCars = 1;

    int number = 0;
    pthread_t entryGuard,exitGuard;
     number = pthread_create(&entryGuard, NULL, entryFunc, NULL);
    if (number != 0)
        cout << "Couldn't create thread: entryGuard" << endl;

    number = pthread_create(&exitGuard, NULL, exitFunc, NULL);
    if (number != 0)
        cout << "Couldn't create thread: exitGuard" << endl;

    pthread_t cars[numberOfCars];
    struct carThreads info[numberOfCars];
    for(int i = 0; i < numberOfCars; i++)
    {
        struct carThreads *p = &info[i];
        p->carNumber = i+1;
        p->delay = rand() % 10;
        number = pthread_create(&cars[i], NULL, carFunc, p);
        if (number != 0)
            cout << "Couldn't create thread: " << i << endl;
    }

    for(int i = 0; i < numberOfCars; i++)
    {
        number = pthread_join(cars[i], NULL);
            cout << "Couldn't join thread: " << i << endl; 

    }

    number = pthread_join(entryGuard, NULL);
    if (number != 0)
        cout << "Couldn't join thread: entryGuard" << endl;

    number = pthread_join(exitGuard, NULL);
    if (number != 0)
        cout << "Couldn't join thread: exitGuard" << endl;

    return 0;
}
void* carFunc(void* arg)
{
    // INITIAZING CAR
    carThreads* car = (carThreads*) arg;
   for (;;)
   {
       pthread_mutex_lock(&mutex);
       carReady = true;
       cout << "CAR#" << car->carNumber << ":" << " Ready To Enter Parking Garage" << endl;
       cout << "CAR#" << car->carNumber << ":" << " Asking Permission From Entry Guard" << endl;
       pthread_cond_signal(&entryCond);
       if(!entryReady)
       {
           pthread_cond_wait(&entryCond,&mutex);
       }
       entryReady = false;
       cout << "CAR#" << car->carNumber << ":" << " Entering Parking Garage" << endl;
       carParked = true;

       // check to close the door
       pthread_cond_signal(&entryCond);
       if(!entryOpen)
       {
           pthread_cond_wait(&entryCond,&mutex);
       }
       entryOpen = false;
    
       // parking and waiting for 3 seconds
       cout << "CAR#" << car->carNumber << ":" << " Parking" << endl;
       cout << "CAR#" << car->carNumber << ":" << " Waiting To Exit" << endl;
       cout << "CAR#" << car->carNumber << ":" << " 3.." << endl;
       sleep(1);
       cout << "CAR#" << car->carNumber << ":" << " 2.." << endl;
       sleep(1);
       cout << "CAR#" << car->carNumber << ":" << " 1.." << endl;
       sleep(1);

       // exiting parking garage
       cout << "CAR#" << car->carNumber << ":" << " Ready To Exit Parking Garage" << endl;
       cout << "CAR#" << car->carNumber << ":" << " Asking Permission From Exit Guard" << endl;
       carLeaving = true;
       pthread_cond_signal(&exitCond);
       if(!exitReady)
       {
           pthread_cond_wait(&exitCond,&mutex);
       }
       exitReady = false;
       cout << "CAR#" << car->carNumber << ":" << " Leaving Parking Garage" << endl;
       carExited = true;

       // check to close the door 
       pthread_cond_signal(&exitCond);
       if(!exitOpen)
       {
           pthread_cond_wait(&exitCond,&mutex);
       }
       exitOpen = false;
       cout << "CAR#" << car->carNumber << ":" << " Exited - Waiting To Start Over" << endl;
       cout << "CAR#" << car->carNumber << " Waiting " << car->delay << " Seconds" << endl;
       for(int i = car->delay; i > 0; i--)
       {
           cout << "CAR#" << car->carNumber << ":" << " " << i << ".." << endl;
           sleep(1);
       }

       pthread_mutex_unlock(&mutex);
   }
    return 0;
}

void* entryFunc(void*)
{
    for (;;)
    {
        // opening entry door
       pthread_mutex_lock(&mutex);
        if(!carReady)
        {
            entryReady = false;
            pthread_cond_wait(&entryCond,&mutex);
        }
        entryReady = true;
        carReady = false;
        cout << "ENTRY GUARD: Permission Granted - Opening Door" << endl;
        pthread_cond_signal(&entryCond);
        pthread_mutex_unlock(&mutex);

        // closing entry door
        pthread_mutex_lock(&mutex);
        if(!carParked)
        {
            entryOpen = false;
            pthread_cond_wait(&entryCond,&mutex);
        }
        entryOpen = true;
        carParked = false;
        cout << "ENTRY GUARD: Car Entered - Closing Door" << endl;
        pthread_cond_signal(&entryCond);
        pthread_mutex_unlock(&mutex);
    }
  
    return 0;
}

void* exitFunc(void*)
{
    for (;;)
    {
       // opening exit door
        pthread_mutex_lock(&mutex);
        if(!carLeaving)
        {
            exitReady = false;
            pthread_cond_wait(&exitCond,&mutex);
        }
        exitReady = true;
        carLeaving = false;
        cout << "EXIT GUARD: Permission Granted - Opening Door" << endl;
        pthread_cond_signal(&exitCond);
        pthread_mutex_unlock(&mutex);

        // closing exit door
        pthread_mutex_lock(&mutex);
        if(!carExited)
        {
            exitOpen = false;
            pthread_cond_wait(&exitCond,&mutex);
        }
        exitOpen = true;
        carExited = false;
        cout << "EXIT GUARD: Car Exited - Closing Door" << endl;
        pthread_cond_signal(&exitCond);
        pthread_mutex_unlock(&mutex);
    }
  
    return 0;
}
