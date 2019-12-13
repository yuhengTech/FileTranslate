/*
	@author:yuheng
	@time:20191101
*/
#define  _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <WinSock2.h>

#define PORT 8087
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 1024
#define FILE_NAME_MAX_SIZE 512
#pragma comment(lib, "WS2_32")
using namespace std;

SOCKET InitNewSocket()//socket初始化函数
{
	//初始化 socket dll
	WSADATA wsaData;
	WORD socketVersion = MAKEWORD(2, 2);
	assert(WSAStartup(socketVersion, &wsaData) == 0);

	//创建socket
	SOCKET c_socket = socket(AF_INET, SOCK_STREAM, 0);
	assert(SOCKET_ERROR != c_socket);

	//指定服务端的地址
	sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.S_un.S_addr = inet_addr(SERVER_IP);
	server_addr.sin_port = htons(PORT);
	assert(SOCKET_ERROR != connect(c_socket, (LPSOCKADDR)&server_addr, sizeof(server_addr)));
	return c_socket;
}
void upload()//客户机到服务器上传文件
{
	SOCKET upload_socket = InitNewSocket();
	//输入文件名
	char FileLocation[FILE_NAME_MAX_SIZE + 1];
	memset(FileLocation, 0, FILE_NAME_MAX_SIZE + 1);
	std::cout << "输入你想上传到服务器的文件：" << std::endl;
	std::cin >> FileLocation;														//这里输入的是本地路径信息

	int file_Location_size = strlen(FileLocation);
	while (FileLocation[file_Location_size] != '\\'&&file_Location_size > 0) { file_Location_size--; }
	char real_name[FILE_NAME_MAX_SIZE];
	memset(real_name, 0, sizeof(real_name));
	strcpy(real_name, FileLocation + file_Location_size + 1);                      //在这里存放真实名字
	cout << "发送到服务器的真实名字：\t" << real_name << endl;						//这个语句是用来测试的

	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	strncpy(buffer,real_name, strlen(real_name) > BUFFER_SIZE ? BUFFER_SIZE : strlen(real_name));  //取较短的,避免溢出

	//向服务器发送文件名
	assert(send(upload_socket, buffer, BUFFER_SIZE, 0) >= 0);

	FILE *fp = fopen(FileLocation, "rb");  //以只读，二进制的方式打开一个文件
	assert(NULL != fp);
	memset(buffer, 0, BUFFER_SIZE);
	int length = 0;

	while ((length = fread(buffer, sizeof(char), BUFFER_SIZE, fp)) > 0)
	{
		assert(send(upload_socket, buffer, length, 0) >= 0);
		memset(buffer, 0, BUFFER_SIZE);
	}
	fclose(fp);
	std::cout << "文件: " << real_name << std::endl << "发送成功 !" << std::endl;
	closesocket(upload_socket);
	//释放 winsock 库
	WSACleanup();

}

void download()
{
	//这是从服务器上下载东西的代码
	SOCKET c_socket = InitNewSocket();
	//输入文件名
	char file_name[FILE_NAME_MAX_SIZE + 1];
	memset(file_name, 0, FILE_NAME_MAX_SIZE + 1);
	std::cout << "输入你想要在服务器上获取的名字：" << std::endl;
	std::cin >> file_name;

	char buffer[BUFFER_SIZE];
	memset(buffer, 0, BUFFER_SIZE);
	strncpy(buffer, file_name, strlen(file_name) > BUFFER_SIZE ? BUFFER_SIZE : strlen(file_name));  //取较短的

	//向服务器发送文件名
	assert(send(c_socket, buffer, BUFFER_SIZE, 0) >= 0);
	
	//打开文件，准备写入
	//这里可以用filename代替
	char receive[FILE_NAME_MAX_SIZE];
	cout << "你想放在哪里？" << endl;
	cin >> receive;
	int file_name_size = strlen(file_name);
	while (file_name[file_name_size] != '\\'&&file_name_size > 0) { file_name_size--; }
	char real_name[FILE_NAME_MAX_SIZE];
	memset(real_name, 0, sizeof(real_name));
	strcpy(real_name, file_name + file_name_size+1);
	cout << "接收到的真实名字" << real_name << endl;
	strcat(receive, real_name);
	FILE *fp = fopen(receive, "wb");  //以只写，二进制的方式打开一个文件
	assert(NULL != fp);
	memset(buffer, 0, BUFFER_SIZE);
	int length = 0;
	while ((length = recv(c_socket, buffer, BUFFER_SIZE, 0)) > 0)
	{
		assert(fwrite(buffer, sizeof(char), length, fp) >= (size_t)length);
		memset(buffer, 0, BUFFER_SIZE);
	}
	std::cout << "接受文件 : " << file_name <<endl<< " 接收成功 !" << std::endl;
	fclose(fp);
	closesocket(c_socket);
	WSACleanup();
}
int main()
{
	upload();//在这里测试上传
}