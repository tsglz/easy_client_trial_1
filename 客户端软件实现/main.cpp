#define ConByUDP

#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<stdio.h>
#include<winsock.h>
#include<string.h>

#define LINELEN 128
#define WSVERS MAKEWORD(2,0)
#define MSG "what daytime is it?\n"

//TCP
#ifdef ConByTCP
#include "connectTCP.h"
#include "connectTCP.cpp"
#include "errexit.h"
#include "connectsock.h"

void TCPdaytime(const char*, const char*);

int main(int argc, char* argv[])
{
	char* host = (char*)"localhost";
	char* service = (char*)"daytime";	//标准的TCP/IP服务可以使用服务名
	WSADATA wsadata;
	switch (argc)
	{
	case 1:
		host = (char*)"localhost";
		break;
	case 3:
		service = argv[2];
	case 2:
		host = argv[1];
		break;
	default:
		fprintf(stderr, "usage: TCPdaytime [host [port]]\n");
		exit(1);
}
	if (WSAStartup (WSVERS, &wsadata) != 0)
		errexit("WSAStartup failed\n");
	TCPdaytime(host, service);
	WSACleanup();
	return 0;
}
void TCPdaytime (const char* host, const char* service)
{
	char buf[LINELEN + 1];
	SOCKET s;
	int cc;
	s = connectTCP(host, service);
	//创建了套接字并完成了和服务器的连接
	cc = recv(s, buf, LINELEN, 0);

	while (cc != SOCKET_ERROR && cc > 0)
	{
		buf[cc] = '\0';
		(void)fputs(buf, stdout);
		cc = recv(s, buf, LINELEN, 0);
	}
	closesocket(s);
}
#endif // ConByTCP


//UDP
#ifdef ConByUDP
#include "errexit.h"
#include "connectsock.h"
#include"connectUDP.h"
#include "connectUDP.cpp"

void UDPdaytime(const char* host, const char* service);

int main(int argc, char* argv[])
{
	char* host = (char*)"localhost";
	char* service = (char*)"daytime";	//标准的TCP/IP服务可以使用服务名
	WSADATA wsadata;
	switch (argc)
	{
	case 1:
		host = (char*)"localhost";
		break;
	case 3:
		service = argv[2];
	case 2:
		host = argv[1];
		break;
	default:
		fprintf(stderr, "usage: UDPPdaytime [host [port]]\n");
		exit(1);
	}
	if (WSAStartup(WSVERS, &wsadata) != 0)
		errexit("WSAStartup failed\n");
	UDPdaytime(host, service);
	WSACleanup();
	return 0;
}
void UDPdaytime(const char* host, const char* service)
{
	char buf[LINELEN + 1];
	SOCKET s;
	int n;

	s = connectUDP(host, service);
	(void)send(s, MSG, strlen(MSG), 0);
	n = recv(s, buf, LINELEN, 0);
	
	//UDP>>数据报
	if (n == SOCKET_ERROR)
		errexit("recv failed: recv() error %d\n", GetLastError());
	else
	{
		buf[n] = '\0';
		(void)fputs(buf, stdout);
	}
	closesocket(s);
}
#endif // !ConByUDP
