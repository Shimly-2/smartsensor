/**
 *  This file is part of my work.                                             *
 *                                                                            *
 *  This program is tu make the TCP Client                                    *
 *                                                                            *
 *  @file     Client.cpp                                                      *
 *  @brief    TCP Client                                                      *
 *  Details.                                                                  *
 *                                                                            *
 *  @author   JYY 1950049                                                     *
 *  @email    1950049@tongji.edu.cn                                           *
 *  @gitee    https://gitee.com/jin-yiyang                                    *
 *  @version  1.0.0.1                                                         *
 *  @date     2022/5/29                                                       *
 *                                                                            *
 *----------------------------------------------------------------------------*
 *  Remark         : Description                                              *
 *----------------------------------------------------------------------------*
 *  Change History :                                                          *
 *  <Date>     | <Version> | <Author>       | <Description>                   *
 *----------------------------------------------------------------------------*
 *  2022/05/29 | 1.0.0.1   | jyy            | Create file                     *
 *----------------------------------------------------------------------------*
 *                                                                            *
 */

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#pragma comment(lib,"ws2_32.lib")
#include <WinSock2.h>
#include <iostream>
#include <stdlib.h>
#include <time.h>

int main()
{
	time_t t;
	struct tm* lt;
	time(&t);//获取Unix时间戳。
	lt = localtime(&t);//转为时间结构。

	/* 定义结构体变量 */
	WSAData wsaData;

	/* 定义套接字版本，初始化套接字库 */
	if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup error", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	printf("[%d:%d:%d]: Start creating TCP client...\n", lt->tm_hour, lt->tm_min, lt->tm_sec);

	/* 创建套接字地址类型 */
	SOCKADDR_IN server; //Adres przypisany do socketu TCP_socket
	
	/*地址结构体变量的大小*/
	int len = sizeof(server); 

	/* 设置ipv4，设置ip地址变量端口，ip地址 */
	server.sin_addr.s_addr = inet_addr("127.0.0.1"); //Addres = localhost
	server.sin_port = htons(8081); //Port = 1111
	server.sin_family = AF_INET; //IPv4 Socket

	/* 定义套接字句柄 */
	SOCKET TCP_socket; 

	/* 定义套接字类型 */
	TCP_socket = socket(AF_INET, SOCK_STREAM, NULL);

	/* 连接server */
	if (connect(TCP_socket, (SOCKADDR*)&server, len) != 0) //TCP_socket
	{
		MessageBoxA(NULL, "Blad TCP_socket", "Error", MB_OK | MB_ICONERROR);
		return 0; 
	}

	while (1)
	{
		char buffer[1024] = "\0";
		time(&t);//获取Unix时间戳。
		lt = localtime(&t);//转为时间结构。
		printf("[%d:%d:%d]: Please input message(input bye to quit): ", lt->tm_hour, lt->tm_min, lt->tm_sec);
		scanf("%s", buffer);
		if (strcmp(buffer, "bye") == 0)
		{
			time(&t);//获取Unix时间戳。
			lt = localtime(&t);//转为时间结构。
			printf("[%d:%d:%d]: TCP client quit\n", lt->tm_hour, lt->tm_min, lt->tm_sec);
			Sleep(100);
			closesocket(TCP_socket);
			break;
		}
		if (sendto(TCP_socket, buffer, sizeof(buffer), 0, (struct sockaddr*) & server, len) != SOCKET_ERROR)
		{
			//printf("sending..\n");
			Sleep(100);
			if (recvfrom(TCP_socket, buffer, sizeof(buffer), 0, (struct sockaddr*) & server, &len) != SOCKET_ERROR)
			{
				time(&t);//获取Unix时间戳。
				lt = localtime(&t);//转为时间结构。
				printf("[%d:%d:%d]: Receive from TCP server: %s\n\n", lt->tm_hour, lt->tm_min, lt->tm_sec, buffer);
			}

		}
	}
	//cleanup
	closesocket(TCP_socket);
	WSACleanup();
	return 0;
}

