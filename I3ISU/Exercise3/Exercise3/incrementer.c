#include <pthread.h>
#include <iostream>
#include <unistd.h>

using namespace std;

static int counter = 0;

void* incrementer(void* x);
void* status(void* x);

int main()
{
    pthread_t inc,stat;
    int *x = 0;
    pthread_create(&inc, nullptr, incrementer, (void *) x);
    pthread_create(&stat, nullptr, status, (void *) x);
    
    pthread_join(inc, nullptr);
    pthread_join(stat, nullptr);
}

void* incrementer(void* x)
{
    for(int i = 0; i < 10; i++)
    {
    counter++;
    sleep(1);
    }
    
    return nullptr;
}

void* status(void* x)
{
    for(int i = 0; i < 10; i++)
    {
    cout << counter << endl;
    sleep(1.1);
    }
    return nullptr;
}
