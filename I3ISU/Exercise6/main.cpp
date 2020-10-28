#include "MsgQueue.h"
#include "Message.h"
#include <unistd.h>

void* receiver(void* arg);
void* sender(void* arg);
void receiverHandler(unsigned long id, Message* msg);

enum
{
    ID_POINT
};

struct Point3D : public Message
{
    int x,y,z;
};

int main()
{
    MsgQueue queue(10);

    pthread_t send,receive;

    pthread_create(&send,nullptr,sender,(void*) &queue);
    pthread_create(&receive,nullptr,receiver,(void*) &queue);

    pthread_join(send,nullptr);
    pthread_join(receive,nullptr);

    cin.get();

    return 0;
}

void* sender(void* arg)
{
    MsgQueue* MyQueuePtr = static_cast<MsgQueue*> (arg);
    for(;;)
    {
        Point3D* pointMsg = new Point3D;
        pointMsg->x = 1;
        pointMsg->y = 2;
        pointMsg->z = 3;
        MyQueuePtr->send(ID_POINT, pointMsg);
        sleep(1);
    }
    return 0;
}

void* receiver(void* arg)
{
    MsgQueue* MyQueuePtr = static_cast<MsgQueue*> (arg);
    unsigned long id;
    for(;;)
    {
        Message* msg = MyQueuePtr->receive(id);
        receiverHandler(id,msg);
        delete msg;
    }
    return 0;
}

void receiverHandler(unsigned long id, Message* msg)
{
    switch(id)
    {
        case ID_POINT:
        {
            Point3D* pointMsg = static_cast<Point3D*> (msg);
            cout << "x: " << pointMsg->x << " y: " << pointMsg->y << " z: " << pointMsg->z << endl;
            break;
        }
        default:
            cout << "ID Not Identified" << endl;
            break;
    }

}