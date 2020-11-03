#include "Message.h"
#include "MsgQueue.h"
#include <iostream>
#include <pthread.h>

struct car
{
    MsgQueue* msgQ;

    int id;
};

struct entryDoorOpenReq : Message
{
    MsgQueue* msgQ;
};

struct entryDoorCloseConfirm : Message
{
    bool check;
};

struct exitDoorOpenReq : Message
{
    MsgQueue* msgQ;
};

struct exitDoorCloseConfirm : Message
{
    bool check;
};

enum
{
    ID_ENTRY_DOOR_REQ = 0,
    ID_ENTRY_DOOR_CFM = 1,
    ID_EXIT_DOOR_OPEN_REQ = 2,
    ID_EXIT_DOOR_OPEN_CFM = 3,
    ID_CAR_ENTRY_CFM = 4,
    ID_CAR_EXIT_CFM = 5,
};


void* carFunction(void* arg);
void* exitFunction(void* arg);
void* entryFunction(void* arg);

int main()
{
    int numberOfCar;
    std::cout << "How many cars would you like to park?" << std::endl;
    std::cin >> numberOfCar;
    numberOfCar = numberOfCar > 0 ? numberOfCar-1 : 1;

    pthread_t entryGuard,exitGuard;
    pthread_create(&entryGuard,nullptr,entryFunction,nullptr);
    pthread_create(&exitGuard,nullptr,exitFunction,nullptr);

    pthread_t car[numberOfCar];
    struct car info[numberOfCar];
    for(int i = 0; i <= numberOfCar; i++)
    {
        struct car *threads = &info[i];
        threads->id = i+1;
        pthread_create(&car[i],nullptr,carFunction,threads);
    }

    pthread_join(entryGuard,nullptr);
    pthread_join(exitGuard,nullptr);

    for(int i = 0; i <= numberOfCar; i++)
    {
        pthread_join(car[i],nullptr);
    }

    std::cin.get();

    return 0;
}

void* carFunction(void* arg)
{
    
    for (;;)
    {

    }
    return 0;
}

void* entryFunction(void* arg)
{

    return 0;
}

void* exitFunction(void* arg)
{

    return 0;
}