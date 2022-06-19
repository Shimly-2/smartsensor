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
	time(&t);//��ȡUnixʱ�����
	lt = localtime(&t);//תΪʱ��ṹ��

	/* ����ṹ����� */
	WSAData wsaData;

	/* �����׽��ְ汾����ʼ���׽��ֿ� */
	if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0)
	{
		MessageBoxA(NULL, "Winsock startup error", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}
	printf("[%d:%d:%d]: Start creating TCP client...\n", lt->tm_hour, lt->tm_min, lt->tm_sec);

	/* �����׽��ֵ�ַ���� */
	SOCKADDR_IN server; //Adres przypisany do socketu TCP_socket
	
	/*��ַ�ṹ������Ĵ�С*/
	int len = sizeof(server); 

	/* ����ipv4������ip��ַ�����˿ڣ�ip��ַ */
	server.sin_addr.s_addr = inet_addr("127.0.0.1"); //Addres = localhost
	server.sin_port = htons(8081); //Port = 1111
	server.sin_family = AF_INET; //IPv4 Socket

	/* �����׽��־�� */
	SOCKET TCP_socket; 

	/* �����׽������� */
	TCP_socket = socket(AF_INET, SOCK_STREAM, NULL);

	/* ����server */
	if (connect(TCP_socket, (SOCKADDR*)&server, len) != 0) //TCP_socket
	{
		MessageBoxA(NULL, "Blad TCP_socket", "Error", MB_OK | MB_ICONERROR);
		return 0; 
	}

	while (1)
	{
		char buffer[1024] = "\0";
		time(&t);//��ȡUnixʱ�����
		lt = localtime(&t);//תΪʱ��ṹ��
		printf("[%d:%d:%d]: Please input message(input bye to quit): ", lt->tm_hour, lt->tm_min, lt->tm_sec);
		scanf("%s", buffer);
		if (strcmp(buffer, "bye") == 0)
		{
			time(&t);//��ȡUnixʱ�����
			lt = localtime(&t);//תΪʱ��ṹ��
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
				time(&t);//��ȡUnixʱ�����
				lt = localtime(&t);//תΪʱ��ṹ��
				printf("[%d:%d:%d]: Receive from TCP server: %s\n\n", lt->tm_hour, lt->tm_min, lt->tm_sec, buffer);
			}

		}
	}
	//cleanup
	closesocket(TCP_socket);
	WSACleanup();
	return 0;
}

