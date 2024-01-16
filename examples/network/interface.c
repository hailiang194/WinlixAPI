#include <stdio.h>
#include <stdlib.h>
#include "api/network/interface.h"

int main(void)
{
    ApiNetworkInterfacesList* list = initNetworkInterfacesList();
    ApiNetworkInterface nwInterface = { 0 };
    while(nextNetworkInterface(list, &nwInterface))
    {
        API_PRINTF(TEXT("Interface name: %s\n"), nwInterface.name);
#ifdef _WIN32
        wprintf(L"\tFriendly name: %s\n", nwInterface.friendlyName);
#else
#endif
    }
    closeNetworkInterfacesList(list);
    return 0;
}