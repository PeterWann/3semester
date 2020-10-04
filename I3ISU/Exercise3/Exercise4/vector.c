#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include "Vector.h"

static void* writer(void* arg);

int main(int argc, char *argv[])
{
    int a = 0;
    Vector vO;
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
    printf("You have chosen to create: %i\n", a);

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
      sleep(1);

    return NULL;
 }
