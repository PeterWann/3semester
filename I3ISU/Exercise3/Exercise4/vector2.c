#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "Vector.h"

static void* writer(void* arg);
  int delay = 0;
  
int main(int argc, char *argv[])
{
    int a = 0;
    int vectorAmount = 0;
    printf("Choose number of threads - (1-100):\n");
    scanf("%d", &a);
    if (a > 100)
    {
        a = 100;
    }
    if (a < 1)
    {
        a = 1;
    }
    printf("You have chosen to create %i threads\n", a);
    
    printf("How much delay would you like in microseconds?\n");
    scanf("%d",&delay);
    printf("You have chosen %i delay in microseconds\n", delay);
    
    printf("How many elements would you like in your vector?\n");
    scanf("%d",&vectorAmount);
    printf("You have chosen %i vector elements\n", vectorAmount);
    
    Vector vO(vectorAmount);
    pthread_t threadCreate[a];

    for (int i = 0; i < a; i++)
    {
        pthread_create(&threadCreate[i], NULL, writer, &vO);
    }

    for (int i = 0; i <a; i++)
    {
    pthread_join(threadCreate[i], NULL);
    }
    return 0;
}

static void * writer(void* arg)
 {
     Vector* vectorTest = (Vector*) arg;

      if(!vectorTest->setAndTest(pthread_self()))
      {
          printf("Error in threads. ID = %lu\n", pthread_self());
      }
      usleep(delay);

    return NULL;
 }
