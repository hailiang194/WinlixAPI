#ifndef _IPC_MESSAGE_QUEUE_H_
#define _IPC_MESSAGE_QUEUE_H_

#ifdef _WIN32
typedef int key_t;
typedef int MessageQueueType;
#else
#include <sys/ipc.h>
typedef int MessageQueueType;
#endif

#include "api/basetype.h"

#define MessageQueueMessageType(typename, size) typedef struct { long int msg_type; unsigned char message[size]; } typename;

START_EXTERN

EXTENDAPI MessageQueueType initMessageQueue(key_t key);
EXTENDAPI BOOL isConnected(MessageQueueType* queue);
EXTENDAPI BOOL receiveMessage(MessageQueueType* queue, void* message, size_t messageSize);
EXTENDAPI BOOL sendMessage(MessageQueueType* queue, void* message, size_t messageSize);
EXTENDAPI BOOL clearMessage(MessageQueueType* queue);

END_EXTERN

#endif //!_IPC_MESSAGE_QUEUE_H_