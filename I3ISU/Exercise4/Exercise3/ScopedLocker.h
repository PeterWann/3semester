#include <pthread.h>

class ScopedLocker
{
public:
    ScopedLocker(pthread_mutex_t &a) : mutex(a)
    {
        pthread_mutex_lock(&mutex);
    }
    ~ScopedLocker()
    {
        pthread_mutex_unlock(&mutex);
    }
private:
    pthread_mutex_t &mutex;
};
