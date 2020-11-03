#include "MsgQueue.h"
#include "Message.h"
#include <unistd.h>

#define ERROR(x) cout << "Error: " << x << endl;

void* receiver(void* arg);
void* sender(void* arg);
void receiverHandler(unsigned long id, Message* msg);

enum
{
    ID1,
    ID2,
    ID3,
};

struct Point3D : public Message
{
    int x,y,z;
};

int main()
{
    MsgQueue queue(10);
    pthread_t send,receive;
    int s = 0;

    s = pthread_create(&send,nullptr,sender,(void*) &queue);
    if (s != 0)
        ERROR("creating thread: send");
    s = pthread_create(&receive,nullptr,receiver,(void*) &queue);
    if (s != 0)
        ERROR("creating thread: receive");

    s = pthread_join(send,nullptr);
    if (s != 0)
        ERROR("joining thread: send");
    s = pthread_join(receive,nullptr);
    if (s != 0)
        ERROR("joining thread: receive");

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
        MyQueuePtr->send(ID1, pointMsg);
        sleep(1);
        Point3D* pointMsg2 = new Point3D;
        pointMsg2->x = 4;
        pointMsg2->y = 5;
        pointMsg2->z = 6;
        MyQueuePtr->send(ID2, pointMsg2);
        sleep(1);

    }
    return 0;
}

void* receiver(void* arg)
{
    MsgQueue* MyQueuePtr = static_cast<MsgQueue*> (arg);
    unsigned long id = 100;
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
        case ID1:
        {
            Point3D* pointMsg1 = static_cast<Point3D*> (msg);
            cout << "x: " << pointMsg1->x << " y: " << pointMsg1->y << " z: " << pointMsg1->z << endl;
            break;
        }
        case ID2:
        {
            Point3D* pointMsg2 = static_cast<Point3D*> (msg);
            cout << "x: " << pointMsg2->x << " y: " << pointMsg2->y << " z: " << pointMsg2->z << endl;
            break;
        }
        default:
            cout << "ID Not Identified" << endl;
            break;
    }

}