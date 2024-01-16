#ifndef _NETWORK_INTERFACE_H_
#define _NETWORK_INTERFACE_H_

#ifdef _WIN32
#include <WinSock2.h>
#include <iphlpapi.h>
#pragma comment(lib, "IPHLPAPI.lib")
#else
#include <sys/types.h>
#include <ifaddrs.h>
#endif
#include "api/basetype.h"

typedef struct
{
#ifdef _WIN32
    ULONG outBufLen;
    PIP_ADAPTER_ADDRESSES pAddresses;
    DWORD lastReturnValue;
    PIP_ADAPTER_ADDRESSES pNextAddress;
#else
    struct ifaddrs *ifaddr;
    struct ifaddrs *ifa;
#endif
} ApiNetworkInterfacesList;

typedef struct
{
    char* name;
#ifdef _WIN32
    PWCHAR friendlyName;
#else
#endif
} ApiNetworkInterface;

START_EXTERN

EXTENDAPI ApiNetworkInterfacesList* initNetworkInterfacesList(void);
EXTENDAPI BOOL nextNetworkInterface(ApiNetworkInterfacesList* list, ApiNetworkInterface* nwInterface);
EXTENDAPI void closeNetworkInterfacesList(ApiNetworkInterfacesList* list);


END_EXTERN

#endif //!_NETWORK_INTERFACE_H_