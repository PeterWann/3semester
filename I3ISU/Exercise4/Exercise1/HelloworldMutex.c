#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void* printFunction(void* myvar);

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

int main()
{
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
    
    return 0;
}
    
void* printFunction(void * myvar)
{
    char *msg;
    msg = (char *) myvar;
    int i;
    
    for(i = 0; i < 10; i++)
    {
        pthread_mutex_lock(&mtx);
        printf("Hello %d %s\n", i, msg);
        pthread_mutex_unlock(&mtx);
        sleep(1);
    }
    
    return NULL;
}
