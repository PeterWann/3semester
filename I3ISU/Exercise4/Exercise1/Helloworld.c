#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>

void* printFunction(void* myvar);

sem_t semaphore;

int main()
{
    sem_init(&semaphore,0,1);
    pthread_t pt, ut;
    char *thread1 = "from thread 0";
    char *thread2 = "from thread 1";
    int ret1, ret2;
    
    ret1 = pthread_create(&pt, NULL, printFunction, (void *) thread1);
    ret2 = pthread_create(&ut, NULL, printFunction, (void *) thread2);
    
    pthread_join(pt, NULL);
    pthread_join(ut, NULL);
    
    if(ret1 == 0)
    {
    printf("Thread 0 terminates\n");
    }
    if(ret2 == 0)
    {
    printf("Thread 1 terminates\n");
    }
    printf("Main: Exiting\n");
    
    sem_destroy(&semaphore);
    
    return 0;
}
    
void* printFunction(void * myvar)
{
    char *msg;
    msg = (char *) myvar;
    int i;
    
    for(i = 0; i < 10; i++)
    {
        sem_wait(&semaphore);
        printf("Hello %d %s\n", i, msg);
        sem_post(&semaphore);
        sleep(1);
    }
    
    return NULL;
}
