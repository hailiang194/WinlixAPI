#include "api/network/interface.h"

#ifdef _WIN32
#define WORKING_BUFFER_SIZE 15000
#define MAX_TRIES 3
#else
#include <stdlib.h>
#include <string.h>
#endif

ApiNetworkInterfacesList* initNetworkInterfacesList(void)
{
    ApiNetworkInterfacesList* list = (ApiNetworkInterfacesList*)malloc(sizeof(ApiNetworkInterfacesList));
    if(list == NULL)
        return NULL;

    memset(list, 0x0, sizeof(ApiNetworkInterfacesList));
#ifdef _WIN32
    list->lastReturnValue = NO_ERROR;
    list->outBufLen = WORKING_BUFFER_SIZE;
    list->pNextAddress = NULL;

    size_t iterations = 0;
    do 
    {
        list->pAddresses = (IP_ADAPTER_ADDRESSES *)malloc(list->outBufLen);
        if(list->pAddresses == NULL)
        {
            closeNetworkInterfacesList(list);
            return NULL;
        }

        list->lastReturnValue = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, list->pAddresses, &(list->outBufLen));

        if(list->lastReturnValue == ERROR_BUFFER_OVERFLOW)
        {
            free(list->pAddresses);
            list->pAddresses = NULL;
        }
        else
        {
            break;
        }

        ++iterations;
    } while((list->lastReturnValue == ERROR_BUFFER_OVERFLOW) && (iterations < MAX_TRIES));

    if(list->lastReturnValue == NO_ERROR)
    {
        list->pNextAddress = list->pAddresses;
    }
    else
    {
        if(list->pAddresses)
            free(list->pAddresses);
    }
#else
    list->ifa = NULL;
    if(getifaddrs(&(list->ifaddr)) == -1)
        return NULL;
    list->ifa = list->ifaddr;
#endif
    return list;
}
BOOL nextNetworkInterface(ApiNetworkInterfacesList* list, ApiNetworkInterface* nwInterface)
{
#ifdef _WIN32
    if(list == NULL || list->pAddresses == NULL || list->pNextAddress == NULL)
        return FALSE;

    nwInterface->name = list->pNextAddress->AdapterName;
    nwInterface->friendlyName = list->pNextAddress->FriendlyName;
    list->pNextAddress = list->pNextAddress->Next;
#else
    if(list == NULL || list->ifaddr == NULL || list->ifa == NULL)
        return FALSE;

    nwInterface->name = list->ifa->ifa_name;

    list->ifa = list->ifa->ifa_next;
#endif
    return TRUE;
}

void closeNetworkInterfacesList(ApiNetworkInterfacesList* list)
{
    if(list == NULL)
        return;
#ifdef _WIN32
    if(list->pAddresses)
        free(list->pAddresses);
#else
    if(list->ifaddr)
        freeifaddrs(list->ifaddr);
#endif    
    free(list);

}
