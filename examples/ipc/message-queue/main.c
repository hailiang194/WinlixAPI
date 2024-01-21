#include <stdio.h>
#include <stdlib.h>
#include "api/string/safe.h"
#include "api/ipc/message-queue.h"

#ifdef _WIN32
#define MAX_INPUT 255
#else
#endif

MessageQueueMessageType(MessageType, MAX_INPUT)

int main(int argc, API_CHAR** argv)
{
    if(argc != 2)
    {
        API_PRINTF(TEXT("\nUsage: %s <1 for sender, 0 for receiver>\n"), argv[0]);
        return (-1);
    }

    BOOL isSender = stringCompare(argv[1], "1", 1) == 0 ? TRUE : FALSE;
    API_PRINTF(TEXT("THIS IS %s\n"), isSender ? TEXT("SENDER"): TEXT("RECEIVER"));
    MessageType msg;
    MessageQueueType queue = initMessageQueue((key_t)1608);

    if(!isConnected(&queue))
    {
        API_PRINTF(TEXT("Unable to connect\n"));
        return 0;
    }

    stringCopy(msg.message, MAX_INPUT, "Hello friend");

    int i = 0;
    while(TRUE)
    {

        if(isSender && sendMessage(&queue, &msg, MAX_INPUT))
        {
            API_PRINTF(TEXT("Message sent\n"));
            ++i;
            if(i == 20)
                break;
        }

        if(!isSender && receiveMessage(&queue, &msg, MAX_INPUT))
        {
            API_PRINTF(TEXT("Message received: %s\n"), (API_CHAR*)msg.message);
            clearMessage(&queue);
        }
    }
    return 0;
}