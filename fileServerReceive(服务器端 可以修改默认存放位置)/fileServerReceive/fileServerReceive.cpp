#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <WinSock2.h>
#include <cassert>
#include <iostream>
#include  <direct.h>
#include "process.h"
using namespace std;

#define PORT 8087
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512
#pragma comment(lib, "WS2_32")
unsigned int _stdcall  Server_accept_upload(void *par);
unsigned int _stdcall  Server_accept_download(void *par);
SOCKET InitNewSocket()
{
	//声明地址结构
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = INADDR_ANY;
	server_addr.sin_port = htons(PORT);

	//初始化 socket dll
	WSADATA wsaData;
	WORD socketVersion = MAKEWORD(2, 2);
	assert(WSAStartup(socketVersion, &wsaData) == 0);

	//创建socket
	SOCKET m_socket = socket(AF_INET, SOCK_STREAM, 0);
	assert(SOCKET_ERROR != m_socket);

	//绑定
	assert(SOCKET_ERROR != bind(m_socket, (LPSOCKADDR)&server_addr, sizeof(server_addr)));

	//监听
	assert(SOCKET_ERROR != listen(m_socket, 5));

	return m_socket;
}
void listenToUpload()
{
	unsigned hThread1;
	unsigned ThreadID1;

	//初始化端Server套接字
	SOCKET UploadServerSocket = InitNewSocket();
	while (1)
	{
		//等待连接
		cout << "等待客户端连接 ..." << endl;
		sockaddr_in client_addr;
		int client_addr_len = sizeof(client_addr);
		SOCKET m_new_socket = accept(UploadServerSocket, (sockaddr *)&client_addr, &client_addr_len);    //创建和客户端的连接
		if (SOCKET_ERROR == m_new_socket)
		{
			cout << "连接失败" << endl;
		}
		else {
			cout << "客户端:"<<inet_ntoa(client_addr.sin_addr)<<"连接成功" << endl;
			hThread1 = _beginthreadex(NULL, 0, Server_accept_upload, (LPVOID)m_new_socket, 0, &ThreadID1);
		}
	}
	closesocket(UploadServerSocket);
	WSACleanup();
}
unsigned int _stdcall  Server_accept_upload(void *par)
{
	SOCKET m_new_socket = SOCKET(par);
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	assert(recv(m_new_socket, buffer, BUFFER_SIZE, 0) >= 0);								//接收客户端发来的文件名
	char file_name[FILE_NAME_MAX_SIZE + 1];
	memset(file_name, 0, FILE_NAME_MAX_SIZE + 1);
	strncpy(file_name, buffer, strlen(buffer) > FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE : strlen(buffer));  //长度取较短的
	std::cout << "接收文件名：" << file_name << endl;

	char test[FILE_NAME_MAX_SIZE + 1] = "c:\\test1\\";
	strcat(test, file_name);

	cout << test << endl;
	FILE *fp = fopen(test, "wb");  //以只写，二进制的方式打开一个文件
	assert(NULL != fp);
	memset(buffer, 0, BUFFER_SIZE);
	int length = 0;
	while ((length = recv(m_new_socket, buffer, BUFFER_SIZE, 0)) > 0)
	{
		assert(fwrite(buffer, sizeof(char), length, fp) >= (size_t)length);
		memset(buffer, 0, BUFFER_SIZE);
	}
	std::cout << "来自客户端的上传文件 : " << file_name << endl << " 上传成功 !" << std::endl;
	fclose(fp);
	closesocket(m_new_socket);
	return 0;
}

void listenToDownload()
{
	unsigned hThread1;
	unsigned ThreadID1;
	//初始化端Server套接字
	SOCKET DownloadServerSocket = InitNewSocket();
	while (1)
	{
		//等待连接
		std::cout << "等待客户端连接 ..." << std::endl;
		sockaddr_in client_addr;
		int client_addr_len = sizeof(client_addr);
		SOCKET m_new_socket = accept(DownloadServerSocket, (sockaddr *)&client_addr, &client_addr_len);
		if (m_new_socket!=INVALID_SOCKET)
		{
			cout << "客户端:" << inet_ntoa(client_addr.sin_addr) << "连接成功" << endl;
			hThread1 = _beginthreadex(NULL, 0, Server_accept_upload, (LPVOID)m_new_socket, 0, &ThreadID1);
		}
		else cout << "连接失败" << endl;
		
	}
	
	closesocket(DownloadServerSocket);
	//释放 winsock 库
	WSACleanup();
}
unsigned int _stdcall  Server_accept_download(void *par)
{
	SOCKET m_new_socket = SOCKET(par);
	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	assert(recv(m_new_socket, buffer, BUFFER_SIZE, 0) >= 0);

	char file_name[FILE_NAME_MAX_SIZE + 1];
	memset(file_name, 0, FILE_NAME_MAX_SIZE + 1);
	strncpy(file_name, buffer, strlen(buffer) > FILE_NAME_MAX_SIZE ? FILE_NAME_MAX_SIZE : strlen(buffer));  //长度取较短的
	std::cout << file_name;

	FILE *fp = fopen(file_name, "rb");  //以只读，二进制的方式打开一个文件
	assert(NULL != fp);
	memset(buffer, 0, BUFFER_SIZE);
	int length = 0;

	while ((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)
	{
		assert(send(m_new_socket, buffer, length, 0) >= 0);
		memset(buffer, 0, BUFFER_SIZE);
	}
	fclose(fp);
	std::cout << "文件: " << file_name << std::endl << "发送成功 !" << std::endl;
	closesocket(m_new_socket);
	return 0;
}

int main()
{
	//listenToDownload();
	//因为时间限制，后续版本再补上自动选择的，目前只能执行一个函数
	listenToUpload();
	return 0;
}