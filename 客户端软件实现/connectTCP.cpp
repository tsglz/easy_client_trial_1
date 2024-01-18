#include<winsock.h>
#include "connectTCP.h"
#include "connectsock.h"

SOCKET connectTCP(const char* host, const char* service)
{
	return connectsock(host, service, "tcp");
}