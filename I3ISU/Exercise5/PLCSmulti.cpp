#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>

using namespace std;

pthread_cond_t                entryCond, exitCond;
pthread_mutex_t mutex;
bool entryReady = false, exitReady = false, entryOpen = false, exitOpen = false, carReady = false, carParked = false, carLeaving = false, carExited = false;
int delay = 0;


void* carFunc(void* arg);
void* entryFunc(void*);
void* exitFunc(void*);

int main()
{
    int number = 0;
    int size = 30;
    pthread_t car1,car2,car3,entryGuard,exitGuard;

    // can allocate size dynamically. 
    const char* one = "1";
    const char* two = "2";
    const char* three = "3";

    number = pthread_create(&car1,NULL,carFunc,(void *)one);
    if (number != 0)
        cout << "Couldn't create thread: CAR #" << one << endl;

    number = pthread_create(&car2,NULL,carFunc,(void *)two);
    if (number != 0)
        cout << "Couldn't create thread: CAR #" << two << endl;
    
    // number = pthread_create(&car2,NULL,carFunc,(void *)three);
    // if (number != 0)
    //     cout << "Couldn't create thread: CAR #" << three << endl;

    number = pthread_create(&entryGuard, NULL, entryFunc, NULL);
    if (number != 0)
        cout << "Couldn't create thread: entryGuard" << endl;

    number = pthread_create(&exitGuard, NULL, exitFunc, NULL);
    if (number != 0)
        cout << "Couldn't create thread: exitGuard" << endl;

    number = pthread_join(car1,NULL);
    number = pthread_join(car2,NULL);
    // number = pthread_join(car3,NULL);

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
   for (;;)
   {
       // entering parking garage
       pthread_mutex_lock(&mutex);
       char* number = (char*) arg;
       carReady = true;
       cout << "CAR#" << number << ":" << " Ready To Enter Parking Garage" << endl;
       cout << "CAR#" << number << ":" << " Asking Permission From Entry Guard" << endl;
       pthread_cond_signal(&entryCond);
       if(!entryReady)
       {
           pthread_cond_wait(&entryCond,&mutex);
       }
       entryReady = false;
       cout << "CAR#" << number << ":" << " Entering Parking Garage" << endl;
       carParked = true;

       // check to close the door
       pthread_cond_signal(&entryCond);
       if(!entryOpen)
       {
           pthread_cond_wait(&entryCond,&mutex);
       }
       entryOpen = false;
    
       // parking and waiting for 3 seconds
       cout << "CAR#" << number << ":" << " Parking" << endl;
       cout << "CAR#" << number << ":" << " Waiting To Exit" << endl;
       cout << "CAR#" << number << ":" << " 3.." << endl;
       sleep(1);
       cout << "CAR#" << number << ":" << " 2.." << endl;
       sleep(1);
       cout << "CAR#" << number << ":" << " 1.." << endl;
       sleep(1);

       // exiting parking garage
       cout << "CAR#" << number << ":" << " Ready To Exit Parking Garage" << endl;
       cout << "CAR#" << number << ":" << " Asking Permission From Exit Guard" << endl;
       carLeaving = true;
       pthread_cond_signal(&exitCond);
       if(!exitReady)
       {
           pthread_cond_wait(&exitCond,&mutex);
       }
       exitReady = false;
       cout << "CAR#" << number << ":" << " Leaving Parking Garage" << endl;
       carExited = true;

       // check to close the door 
       pthread_cond_signal(&exitCond);
       if(!exitOpen)
       {
           pthread_cond_wait(&exitCond,&mutex);
       }
       exitOpen = false;
       cout << "CAR#" << number << ":" << " Exited - Waiting To Start Over" << endl;

       // waiting 3 seconds to start over
       cout << "CAR#" << number << ":" << " 3.." << endl;
       sleep(1);
       cout << "CAR#" << number << ":" << " 2.." << endl;
       sleep(1);
       cout << "CAR#" << number << ":" << " 1.." << endl;
       sleep(1);
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
