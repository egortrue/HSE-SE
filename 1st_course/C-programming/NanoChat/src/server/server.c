
#pragma comment(lib, "Ws2_32.lib")
#define HAVE_STRUCT_TIMESPEC

#include <pthread.h>
#include <winsock.h>
#include <stdlib.h>
#include <stdio.h>

pthread_mutex_t mutex;

void* ClientStart(void* param)
{
	SOCKET client = (SOCKET)param;
	
	// Receive the data from client
	char recieve[1024];
	int ret;
	ret = recv(client, recieve, 1024, 0);
	if (!ret || ret == SOCKET_ERROR)
	{
		pthread_mutex_lock(&mutex);
		printf("Error getting data\n");
		pthread_mutex_unlock(&mutex);
		return (void*)1;
	}
	recieve[ret] = '\0';


	// Print to server's console
	pthread_mutex_lock(&mutex); 
	printf("%s\n", recieve);
	pthread_mutex_unlock(&mutex);


	// Send the data to client
	char transmit[1024];
	sprintf_s(transmit, (size_t)1024, "Your data:\n%s\nwere received\0", recieve);
	ret = send(client, transmit, strlen(transmit)+1, 0);
	if (ret == SOCKET_ERROR)
	{
		pthread_mutex_lock(&mutex);
		printf("Error sending data\n");
		pthread_mutex_unlock(&mutex);
		return (void*)2;
	}


	return (void*)0;
}

SOCKET ServerCreate()
{
	// Create a listening socket
	SOCKET server = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);  // AF_INET - IPv4 address family
	if (server == INVALID_SOCKET)                              // SOCK_STREAM - TCP protocol
	{
		printf("Error create server\n");
		exit(1);
	}

	// Define the socket's address
	SOCKADDR_IN server_addr;
	server_addr.sin_family = AF_INET; 
	server_addr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server_addr.sin_port = htons(5510); // server port, must be more than 1024


	// Bind the listening socket using the
	// information in the sockaddr structure
	if (bind(server, (SOCKADDR*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR)
	{
		printf("Can't start server\n");
		exit(2);
	}

	printf("Server is started\n");
	return server;
}


void ServerRun(SOCKET server)
{
	SOCKET client;
	SOCKADDR_IN client_addr;
	int client_addr_size = sizeof(client_addr);

	const time_t time_start = time(NULL); // Server startup time
	listen(server, 50);                   // Create a waiting line for 50 clients
	pthread_mutex_init(&mutex, NULL);     // Init access switcher for console 

	printf("%s\n", ctime(&time_start));

	// Listen for incoming connection requests on the server
	while (1)
	{	
		// Get client data
		client = accept(server, (SOCKADDR*)&client_addr, &client_addr_size);
		if (client == INVALID_SOCKET)
		{
			printf("Error accept client\n");
			continue;
		}

		// Create a thread for this client
		pthread_t mythread;
		pthread_create(&mythread, NULL, ClientStart, (void*)client);
		pthread_detach(mythread);
	}
	
	pthread_mutex_destroy(&mutex); // Destroy access switcher for console 
	printf("Server is stopped\n");
	closesocket(server);
}

int main()
{
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(1, 1), &wsd)) return 1;		
	printf("Connected to socket lib\n");
	
	SOCKET server = ServerCreate();
	ServerRun(server);

	WSACleanup(); // It terminates use of the Winsock2 DLL (Ws2_32.dll).
	return 0;
}

