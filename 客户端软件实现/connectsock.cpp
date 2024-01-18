#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <winsock.h>

#include "errexit.h"
#include "connectsock.h"

#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff
#endif

SOCKET connectsock(const char* host, const char* service, const char* transport)
{
    struct hostent* phe;	
    struct servent* pse;	
    struct protoent* ppe;	
    struct sockaddr_in sin;	
    int s, type;	

    memset(&sin, 0, sizeof(sin));
    sin.sin_family = AF_INET;

    //服务名到端口号的解析
    if (pse = getservbyname(service, transport))//服务名>服务号
        sin.sin_port = pse->s_port;
    else if ((sin.sin_port = htons((u_short)atoi(service))) == 0)
        errexit("can't get \"%s\" service entry\n", service);
    //域名>IP地址
    if (phe = gethostbyname(host))
        memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
    else if ((sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE)
        errexit("can't get \"%s\" host entry\n", host);
    //协议名>协议号
    if ((ppe = getprotobyname(transport)) == 0)
        errexit("can't get \"%s\" protocol entry\n", transport);

    //根据协议不同，创建不同类型的套接字
    if (strcmp(transport, "udp") == 0)
        type = SOCK_DGRAM;
    else
        type = SOCK_STREAM;

    s = socket(PF_INET, type, ppe->p_proto);
    if (s == INVALID_SOCKET)
        errexit("can't create socket:%d\n", GetLastError());

    //连接套接字
    if (connect(s, (struct sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
        errexit("can't conncet to %s.%s:%d\n", host, service, GetLastError());
    return s;
}
