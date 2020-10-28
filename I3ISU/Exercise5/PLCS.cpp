#include <iostream>
#include <pthread.h>
#include <unistd.h>

using namespace std;

pthread_cond_t                cond1, cond2;
pthread_mutex_t               mtx;
bool entryReady = false, exitReady = false, entryOpen = false, exitOpen = false, carReady = false, carParked = false, carLeaving = false, carExited = false;

void* carFunc(void*);
void* entryFunc(void*);
void* exitFunc(void*);

int main()
{
    pthread_t car,entryGuard,exitGuard;
    int s = 0;

    s = pthread_create(&car, NULL, carFunc, NULL);
    if (s != 0)
        cout << "Couldn't create thread: CAR" << endl;

    s = pthread_create(&entryGuard, NULL, entryFunc, NULL);
    if (s != 0)
        cout << "Couldn't create thread: entryGuard" << endl;

    s = pthread_create(&exitGuard, NULL, exitFunc, NULL);
    if (s != 0)
        cout << "Couldn't create thread: exitGuard" << endl;

    s = pthread_join(car, NULL);
    if (s != 0)
        cout << "Couldn't join thread: CAR" << endl;

    s = pthread_join(entryGuard, NULL);
    if (s != 0)
        cout << "Couldn't join thread: entryGuard" << endl;

    s = pthread_join(exitGuard, NULL);
    if (s != 0)
        cout << "Couldn't join thread: exitGuard" << endl;

    return 0;
}
void* carFunc(void*)
{
   for (;;)
   {
       // entering parking garage
       pthread_mutex_lock(&mtx);
       carReady = true;
       cout << "CAR: Ready To Enter Parking Garage" << endl;
       cout << "CAR: Asking Permission From Entry Guard" << endl;
       pthread_cond_signal(&cond1);
       if(!entryReady)
       {
           pthread_cond_wait(&cond1,&mtx);
       }
       cout << "CAR: Entering Parking Garage" << endl;
       carParked = true;

       // check to close the door
       pthread_cond_signal(&cond1);
       if(!entryOpen)
       {
           pthread_cond_wait(&cond1,&mtx);
       }
       
       // parking and waiting for 3 seconds
       cout << "CAR: Parking" << endl;
       cout << "CAR: Waiting To Exit" << endl;
       cout << "CAR: 3.." << endl;
       sleep(1);
       cout << "CAR: 2.." << endl;
       sleep(1);
       cout << "CAR: 1.." << endl;
       sleep(1);

       // exiting parking garage
       cout << "CAR: Ready To Exit Parking Garage" << endl;
       cout << "CAR: Asking Permission From Exit Guard" << endl;
       carLeaving = true;
       pthread_cond_signal(&cond2);
       if(!exitReady)
       {
           pthread_cond_wait(&cond2,&mtx);
       }
       cout << "CAR: Leaving Parking Garage" << endl;
       carExited = true;

       // check to close the door 
       pthread_cond_signal(&cond2);
       if(!exitOpen)
       {
           pthread_cond_wait(&cond2,&mtx);
       }
       cout << "CAR: Exited - Waiting To Start Over" << endl;

       // waiting 3 seconds to start over
       cout << "CAR: 3.." << endl;
       sleep(1);
       cout << "CAR: 2.." << endl;
       sleep(1);
       cout << "CAR: 1.." << endl;
       sleep(1);
       pthread_mutex_unlock(&mtx);
   }
    return 0;
}

void* entryFunc(void*)
{
    for (;;)
    {
        // opening entry door
        pthread_mutex_lock(&mtx);
        if(!carReady)
        {
            entryReady = false;
            pthread_cond_wait(&cond1,&mtx);
        }
        entryReady = true;
        carReady = false;
        cout << "ENTRY GUARD: Permission Granted - Opening Door" << endl;
        pthread_cond_signal(&cond1);
        pthread_mutex_unlock(&mtx);

        // closing entry door
        pthread_mutex_lock(&mtx);
        if(!carParked)
        {
            entryOpen = false;
            pthread_cond_wait(&cond1,&mtx);
        }
        entryOpen = true;
        carParked = false;
        cout << "ENTRY GUARD: Car Entered - Closing Door" << endl;
        pthread_cond_signal(&cond1);
        pthread_mutex_unlock(&mtx);
    }
  
    return 0;
}

void* exitFunc(void*)
{
    for (;;)
    {
        // opening exit door
        pthread_mutex_lock(&mtx);
        if(!carLeaving)
        {
            exitReady = false;
            pthread_cond_wait(&cond2,&mtx);
        }
        exitReady = true;
        carLeaving = false;
        cout << "EXIT GUARD: Permission Granted - Opening Door" << endl;
        pthread_cond_signal(&cond2);
        pthread_mutex_unlock(&mtx);

        // closing exit door
        pthread_mutex_lock(&mtx);
        if(!carExited)
        {
            exitOpen = false;
            pthread_cond_wait(&cond2,&mtx);
        }
        exitOpen = true;
        carExited = false;
        cout << "EXIT GUARD: Car Exited - Closing Door" << endl;
        pthread_cond_signal(&cond2);
        pthread_mutex_unlock(&mtx);
    }
  
    return 0;
}
