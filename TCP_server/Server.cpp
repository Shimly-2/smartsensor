/**
 *  This file is part of my work.                                             *
 *                                                                            *
 *  This program is tu make the TCP Server                                    *
 *                                                                            *
 *  @file     Server.cpp                                                      *
 *  @brief    TCP Server                                                      *
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
#include <string>
#include <time.h>
#include <fstream> // for ifstream
#include <sstream>

int main()
{
	//char a[] = "-4.3633230961859226E-4";
	//char b[] = "7.539e3";
	//char c[] = "0.22e0";
	//printf("a=%lf\nb=%lf\nc=%lf\n", strtod(a, NULL), strtod(b, NULL), strtod(c, NULL));

	time_t t;
	struct tm* lt;
	time(&t);//��ȡUnixʱ�����
	lt = localtime(&t);//תΪʱ��ṹ��

	/* ����ṹ����� */
	WSAData wsaData;

	/* �����׽��ְ汾����ʼ���׽��ֿ� */
	if (WSAStartup(MAKEWORD(2, 1), &wsaData) != 0)
	{
		MessageBoxA(NULL, "WinSock startup error", "Error", MB_OK | MB_ICONERROR);
		return 0;
	}
	printf("[%d:%d:%d]: Start creating TCP server...\n", lt->tm_hour, lt->tm_min, lt->tm_sec);

	/* �����׽��ֵ�ַ���� */
	SOCKADDR_IN local; 
	SOCKADDR_IN from;

	/* ��ַ�ṹ������Ĵ�С */
	int locallen = sizeof(local);
	int fromlen = sizeof(from);

	/* ����ipv4������ip��ַ�����˿ڣ�ip��ַ */
	local.sin_addr.s_addr = inet_addr("127.0.0.1"); //target PC
	local.sin_port = htons(8081); // sever Port
	local.sin_family = AF_INET; //IPv4 Socket

	/* �����׽��־�� */
	SOCKET TCP_socket; 

	/* �����׽������� */
	TCP_socket = socket(AF_INET, SOCK_STREAM, NULL);

	/* �󶨶�Ӧ�ĵ�ַ���˿ں� */
	bind(TCP_socket, (SOCKADDR*)&local, sizeof(local)); 

	/* �������� */
	listen(TCP_socket, SOMAXCONN); 

	/* ����accept */
	int input_num, output_num;
	SOCKET new_TCP_socket; //build a new socket do new connection. the TCP_socket is just listenning not used to exchange data
	new_TCP_socket = accept(TCP_socket, (SOCKADDR*)&local, &locallen); //newConnection is used to exchange data with client
	int m1 = 0,m2 = 0;
	int k1 = 0,k2 = 0;
	int d1 = 0,d2 = 0;
	double gyro[6000][3];
	double acc[6000][3];
	char buffer[5000] = "\0";
	char buffer2[430000] = "\0";
	int buflen = 0;
	int flag = 0;
	/* ѭ������ */
	while (1)
	{
		if (recvfrom(new_TCP_socket, buffer, sizeof(buffer), 0, (struct sockaddr*) & local, &locallen) != SOCKET_ERROR)
		{
			for (int j = 0; j < strlen(buffer); j++)
			{
				buffer2[buflen++] = buffer[j];
				if (buffer[j] == '$') flag = 1;
			}
			if (flag==1)break;
			//printf("%s",buffer2);
		}
	}

	if (buffer2[0] == 'g' && buffer2[1] == 'y')
	{
		char temp[25] = "\0";
		for (int i = 19; i < strlen(buffer2); i++)
		{
			temp[m1++] = buffer2[i];
			//printf("0:%c\n", buffer[i]);
			if (buffer2[i + 1] == ',') {
				//printf("1:%s\n", temp);
				gyro[k1][d1++] = strtod(temp, NULL);
				for (int j = 0; j < 25; j++)
					temp[j] = '\0';
				m1 = 0;
				i++;
			}
			if (buffer2[i + 1] == ')') {
				//printf("2:%s\n", temp);
				gyro[k1][d1] = strtod(temp, NULL);
				for (int j = 0; j < 25; j++)
					temp[j] = '\0';
				m1 = 0;
				k1++;
				d1 = 0;
				//printf("2:%d\n", k1);
				i = i + 4;
			}
		}
		for (int dd = 0; dd < k1; dd++)
		{
			printf("[%d]:%lf,%lf,%lf\n", dd, gyro[dd][0], gyro[dd][1], gyro[dd][2]);

		}
		printf("wirting gyro.csv...\n");
		std::ofstream outFile;
		outFile.open("gyro.csv", std::ios::out); // ��ģʽ��ʡ��
		for (int j = 0; j < k1; j++)
		{
			for (int i = 0; i < 3; i++)
			{
				if (i < 2)
				{
					outFile << gyro[j][i] << ',';
				}
				else
				{
					outFile << gyro[j][i];
				}
			}
			outFile << '\n';
		}
		outFile.close();
	}


	for (int ddd = 0; ddd < d1 + 1; ddd++)
	{
		buffer2[ddd] = '\0';
	}
	SOCKET new_TCP_socket2; //build a new socket do new connection. the TCP_socket is just listenning not used to exchange data
	new_TCP_socket2 = accept(TCP_socket, (SOCKADDR*)& local, &locallen); //newConnection is used to exchange data with client
	int buflen2 = 0;
	int flag2 = 0;
	/* ѭ������ */
	while (1)
	{
		if (recvfrom(new_TCP_socket2, buffer, sizeof(buffer), 0, (struct sockaddr*) & local, &locallen) != SOCKET_ERROR)
		{
			time(&t);//��ȡUnixʱ�����
			lt = localtime(&t);//תΪʱ��ṹ��

			for (int j = 0; j < strlen(buffer); j++)
			{
				buffer2[buflen2++] = buffer[j];
				if (buffer[j] == '$') flag2 = 1;
			}
			if (flag2 == 1)break;
		}

	}
	
	if (buffer2[0] == 'a' && buffer2[1] == 'c')
	{
		char temp[25] = "\0";
		for (int i = 18; i < strlen(buffer2); i++)
		{
			temp[m2++] = buffer2[i];
			//printf("0:%c\n", buffer[i]);
			if (buffer2[i + 1] == ',') {
				//printf("1:%s\n", temp);
				acc[k2][d2++] = strtod(temp, NULL);
				for (int j = 0; j < 25; j++)
					temp[j] = '\0';
				m2 = 0;
				i++;
			}
			if (buffer2[i + 1] == ')') {
				//printf("2:%s\n", temp);
				acc[k2][d2] = strtod(temp, NULL);
				for (int j = 0; j < 25; j++)
					temp[j] = '\0';
				m2 = 0;
				k2++;
				d2 = 0;
				i = i + 4;
			}
		}
		for (int dd = 0; dd < k2; dd++)
		{
			printf("[%d]:%lf,%lf,%lf\n", dd, acc[dd][0], acc[dd][1], acc[dd][2]);

		}
		printf("wirting acc.csv...\n");
		std::ofstream outFile;
		outFile.open("acc.csv", std::ios::out); // ��ģʽ��ʡ��
		for (int j = 0; j < k2; j++)
		{
			for (int i = 0; i < 3; i++)
			{
				if (i < 2)
				{
					outFile << gyro[j][i] << ',';
				}
				else
				{
					outFile << gyro[j][i];
				}
			}
			outFile << '\n';
		}
		outFile.close();
	}

	while(1){ }
	//cleanup
	closesocket(TCP_socket);
	closesocket(new_TCP_socket);
	WSACleanup();
	return 0;
}