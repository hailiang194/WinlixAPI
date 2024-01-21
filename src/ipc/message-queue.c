#include "api/ipc/message-queue.h"

#ifdef _WIN32
#else
#include <sys/types.h>
#include <sys/msg.h>
#endif

MessageQueueType initMessageQueue(key_t key)
{
    MessageQueueType queue;
#ifdef _WIN32
#else
    queue = msgget(key, 0666 | IPC_CREAT);
#endif
    return queue;
}

BOOL isConnected(MessageQueueType* queue)
{
    return (queue != NULL) && 
#ifdef _WIN32
    TRUE;
#else
    ((*queue) != -1);
#endif
}

BOOL receiveMessage(MessageQueueType* queue, void* message, size_t messageSize)
{
#ifdef _WIN32
#else
    return msgrcv(*queue, message, messageSize, 0, 0) != -1;
#endif
    return TRUE;
}

BOOL sendMessage(MessageQueueType* queue, void* message, size_t messageSize)
{
#ifndef _WIN32
    *((long int*)message) = 1;
    return msgsnd(*queue, message, messageSize, 0) != -1;
#else
#endif
    return TRUE;
}

BOOL clearMessage(MessageQueueType* queue)
{
#ifndef _WIN32
#else
#endif
    return TRUE;
}