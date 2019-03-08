#include<iostream>
#include <WINSOCK2.H>
#pragma comment(lib,"ws2_32.lib")

bool CheckNetwork()
{
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;

	wVersionRequested = MAKEWORD(1, 1);    //初始化Socket动态连接库,请求1.1版本的winsocket库

	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0) {
		return FALSE;
	}

	if (LOBYTE(wsaData.wVersion) != 1 ||   //判断请求的winsocket是不是1.1的版本
		HIBYTE(wsaData.wVersion) != 1) {
		WSACleanup();			//清盘
		return FALSE;					//终止对winsocket使用
	}

	char http[60] = "www.baidu.com";			//访问谷歌网页
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);//建立socket
	if (sock == INVALID_SOCKET)
		return FALSE;
	sockaddr_in hostadd;
	hostent* host = gethostbyname(http);//取得主机的IP地址
	if (host == NULL)
		return FALSE;
	memcpy(&hostadd, host->h_addr, sizeof(hostadd));//将返回的IP信息Copy到地址结构
	hostadd.sin_family = AF_INET;
	hostadd.sin_port = htons(80);

	int time = 1000;	//超时时间
	setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&time, sizeof(time));


	if (connect(sock, (sockaddr*)&hostadd, sizeof(hostadd)) == SOCKET_ERROR)//连接请求
		return FALSE;

	closesocket(sock);
	WSACleanup();
	return TRUE;
}



int main()
{
	while (1)
	{
		int a = CheckNetwork();
	}
	return 0;
}

