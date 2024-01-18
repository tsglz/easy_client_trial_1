#include<stdarg.h>
#include<stdio.h>
#include<stdlib.h>
#include<winsock.h>
#include "errexit.h"

void errexit(const char* format, ...)
{
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	WSACleanup();	//异常退出也要清除动态链接库资源占用
	exit(1);
}