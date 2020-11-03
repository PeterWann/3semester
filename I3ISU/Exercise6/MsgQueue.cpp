#include "MsgQueue.h"

MsgQueue::MsgQueue(unsigned long maxSize)
{
    maxSize_ = maxSize;
}

void MsgQueue::send(unsigned long id, Message* msg)
{
	pthread_mutex_lock(&mutex);

    // Hvis queue er fuld, vent på plads
	while (container.size() >= maxSize_)
	{
		pthread_cond_wait(&txCond,&mutex);
	}

    // Opret struct og tildel data
	msgEvent newMsg;
	newMsg.id_ = id;
	newMsg.msg_ = msg;

    // Kom den ind i queue
	container.push(newMsg);

    // Signaler receiver
	pthread_cond_signal(&rxCond);
	pthread_mutex_unlock(&mutex);
}

Message* MsgQueue::receive(unsigned long& id)
{
	pthread_mutex_lock(&mutex);

    // Hvis queue er tom, vent på besked
	while (container.size() < 1)
	{
		pthread_cond_wait(&rxCond, &mutex);
	}

    // Tag ny data fra queue
	id = container.front().id_;
	Message* msg = container.front().msg_;

    // Fjern ny data fra queue
	container.pop();

    // Signaler send
	pthread_cond_broadcast(&txCond);
	pthread_mutex_unlock(&mutex);

	return msg;
}

MsgQueue::~MsgQueue()
{
    // Nedlæg mutex og conditions
	delete []&container;
	pthread_mutex_destroy(&mutex);
	pthread_cond_destroy(&rxCond);
	pthread_cond_destroy(&txCond);
}
