#include "server.h"

FILE* file_log;
pthread_mutex_t mutex_log;

int main()
{
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(1, 1), &wsd)) return 1;		

	file_log = FileOpen("data/log.txt", "a");
	pthread_mutex_init(&mutex_log, NULL); // Init access switcher for log file 

	// Create server and thread for its
	SERVER* server = ServerCreate();
	pthread_create(&server->thread, NULL, ServerRun, (void*)server);

	// Main server's console cycle
	while (1)
	{
		printf("<server> ");
		char command[51] = {0};
		gets_s(command, 50);

		if (!strcmp(command, "help"))
		{
			printf(" help - list of commands\n");
			printf("  cls - clear screen\n");
			printf(" stop - shutdown the server\n");
			continue;
		}

		if (!strcmp(command, "cls"))
		{
			system("cls");
			continue;
		}

		if (!strcmp(command, "stop"))
		{
			ServerStop(server);
			ServerDestroy(server);
			break;
		}

		if (strlen(command))
			printf("Command not found. Use \"help\" to show commands\n");
	}

	pthread_mutex_destroy(&mutex_log); // Destroy access switcher for log file
	WSACleanup(); // It terminates use of the winsock library (Ws2_32.dll).
	return 0;
}

//=================================================================================
// Server definition

SERVER* ServerCreate()
{
	SERVER* server = (SERVER*)malloc(sizeof(SERVER));
	if (!server) exit(EXIT_FAILURE);


	// Define the server's socket
	server->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);  // AF_INET - IPv4 address family
	if (server->socket == INVALID_SOCKET)                       // SOCK_STREAM - TCP protocol
	{
		printf("Error create server\n");
		free(server);
		exit(EXIT_FAILURE);
	}


	// Define the server's address
	server->address.sin_family = AF_INET;
	server->address.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	server->address.sin_port = htons(5510); // server port, must be more than 1024


	// Bind the socket using the information in the sockaddr structure
	if (bind(server->socket, (SOCKADDR*)&server->address, sizeof(server->address)) == SOCKET_ERROR)
	{
		printf("Can't start server\n");
		free(server);
		exit(EXIT_FAILURE);
	}
	

	// Success
	printf("Server is started\n");
	return server;
}

void* ServerDestroy(SERVER* server)
{
	if (server->status == started)
		ServerStop(server);
	closesocket(server->socket);
	free(server);
	return NULL;
}

//=================================================================================
// Server thread

void* ServerRun(void* server_param)
{
	SERVER* server = (SERVER*)server_param;

	SOCKET client;
	SOCKADDR_IN client_addr;
	int client_addr_size = sizeof(client_addr);

	listen(server, 50); // Create a waiting line for 50 clients

	// Server startup time	
	char time_str_start[100];
	const time_t time_raw_start = time(NULL);
	ctime_s(time_str_start, sizeof(time_str_start), &time_raw_start);
	char border[] = "==============================================\n";
	char log_time[1000];
	sprintf_s(log_time, (size_t)1024, "%s\nServer was started at %s\n%s", border, time_str_start, border);
	LogWrite(log_time);

	// Listen for incoming connection requests on the server
	while (1)
	{
		// Get client data
		client = accept(server, (SOCKADDR*)&client_addr, &client_addr_size);
		if (client == INVALID_SOCKET)
		{
			//fprintf(log_file, "Error accept client\n");
			continue;
		}

		// Create a thread for this client
		pthread_t client_thread;
		pthread_create(&client_thread, NULL, ClientRun, (void*)client);
		pthread_detach(client_thread);

		pthread_testcancel();
	}
}

void ServerStop(SERVER* server)
{
	if (pthread_cancel(server->thread))
		pthread_join(server->thread, NULL);
	server->status = closed;
	printf("Server is closed\n");
}

//=================================================================================

void* ClientRun(void* client_param)
{
	SOCKET client = (SOCKET)client_param;
	int data_size;

	// Get the data from client
	char receive[MAX_DATA_SIZE];
	data_size = recv(client, receive, MAX_DATA_SIZE, 0);
	if (!data_size || data_size == SOCKET_ERROR)
	{
		LogWrite("Error getting data\n");
		return (void*)1;
	}
	strcat(receive, "\n");
	LogWrite(receive);


	// Send the data to client
	char transmit[MAX_DATA_SIZE];
	sprintf_s(transmit, (size_t)MAX_DATA_SIZE, "Your data:\n%s\nwere received", receive);
	data_size = send(client, transmit, strlen(transmit), 0);
	if (data_size == SOCKET_ERROR)
	{
		LogWrite("Error sending data\n");
		return (void*)2;
	}

	return (void*)0;
}

//=================================================================================

FILE* FileOpen(const char* name, const char* mode)
{
	FILE* file;
	fopen_s(&file, name, mode);

	if (!file)
	{
		if (*mode == 'r')
			puts("\nERROR: The file was not opened for reading");
		else if (*mode == 'w')
			puts("\nERROR: The file was not opened for writing");
		else if (*mode == 'a')
			puts("\nERROR: The file was not opened for addtion");
		exit(EXIT_FAILURE);
	}
	return file;
}

void LogWrite(char* string)
{
	pthread_mutex_lock(&mutex_log);
	fprintf(file_log, string);
	pthread_mutex_unlock(&mutex_log);
}

//=================================================================================
