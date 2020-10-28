#include <pthread.h>
#include <iostream>

using namespace std;

class ScopedLocker
{
public:
    ScopedLocker(pthread_mutex_t &a) : mutex(a)
    {
        pthread_mutex_lock(&mutex);
        cout << "Scoped locks" << endl;
    }
    ~ScopedLocker()
    {
        pthread_mutex_unlock(&mutex);
        cout << "Scoped unlocks" << endl;
    }
private:
    pthread_mutex_t &mutex;
};
