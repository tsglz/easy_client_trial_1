#include<winsock.h>
#include "connectUDP.h"
#include "connectsock.h"

SOCKET connectUDP(const char* host, const char* service)
{
	return connectsock(host, service, "udp");
}
