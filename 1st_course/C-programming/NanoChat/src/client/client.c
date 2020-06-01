﻿#include "client.h"

void SendData2Server(int count, int number)
{

	// Create client
	SOCKET client;
	client = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (client == INVALID_SOCKET)
	{
		printf("Error create socket\n");
		return;
	}

	// Connect to server
	SOCKADDR_IN server;
	server.sin_family = AF_INET;
	server.sin_port = htons(5510); // server port
	server.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); // lookup | address of your own network adapter
	if (connect(client, (SOCKADDR*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("Can't connect to server\n");
		closesocket(client);
		return;
	}

	// Send message
	char message[1024];
	sprintf_s(message, (size_t)1024, "<%d client> test %d\0", number, count);
	int ret = send(client, message, strlen(message)+1, 0);
	if (ret == SOCKET_ERROR)
	{
		printf("Can't send message\n");
		closesocket(client);
		return;
	}
	printf("Send: %s\nbytes: %d\n\n", message, ret);


	// Receive the answer from server
	//ret = SOCKET_ERROR;
	//while (ret == SOCKET_ERROR)
	//{
	//	// Receive the text
	//	ret = recv(client, message, 1024, 0);
	//	message[ret] = '\0';
	//	if (ret == 0 || ret == WSAECONNRESET)
	//	{
	//		printf("Connection closed\n");
	//		break;
	//	}

	//	if (ret < 0)
	//	{
	//		//printf("Can't receive message\n");
	//		//closesocket(client);
	//		//return;
	//		continue;
	//	}

	//	// Output to local console
	//	printf("Recieve:\n#############\n%s\nbytes: %d\n#############\n\n", message, ret);
	//}

	closesocket(client);
}

int main()
{
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(1, 1), &wsd)) return 1;


		
	WSACleanup(); // It terminates use of the Winsock2 DLL (Ws2_32.dll).
	return 0;
}

