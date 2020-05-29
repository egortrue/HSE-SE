#include "server.h"

FILE* file_log;
pthread_mutex_t mutex_log;
pthread_mutex_t mutex_cmd;

int main()
{
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(1, 1), &wsd)) return 1;		

	char file_log_path[] = "..\\src\\server\\data\\log.txt";
	file_log = FileOpen(file_log_path, "a");
	pthread_mutex_init(&mutex_log, NULL); // Init access switcher for log file 
	pthread_mutex_init(&mutex_cmd, NULL); // Init access switcher for command line 

	SERVER* server = ServerCreate();

	// Main server's console cycle
	while (1)
	{
		printf("<server> ");
		char command[51] = { 0 };
		gets_s(command, 50);

		if (!strcmp(command, "help"))
		{
			printf("\n");
			printf("|   help - list of commands\n");
			printf("|    cls - clear screen\n");
			printf("| status - show server's status\n");
			printf("|  start - start the server\n");
			printf("|   stop - shutdown the server\n");
			printf("|   exit - destroy the server and exit\n");
			printf("\n");
			continue;
		}

		if (!strcmp(command, "cls"))
		{
			system("cls");
			continue;
		}

		if (!strcmp(command, "status"))
		{
			if (server->status == started)
				printf("Server is started\n");
			else
				printf("Server is stopped\n");
			continue;
		}

		if (!strcmp(command, "start"))
		{
			if (server->status == stopped)
			{
				pthread_create(&server->thread, NULL, ServerRun, (void*)server);
				printf("Server is started\n");
			}
			else
				printf("Server already started\n");
			continue;
		}

		if (!strcmp(command, "stop"))
		{
			if (server->status == started)
			{
				ServerStop(server);
				printf("Server is stopped\n");
			}
			else
				printf("Server already stopped\n");
			continue;
		}

		if (!strcmp(command, "exit"))
			break;

		printf("Command not found. Use \"help\" to show commands\n");
	}

	ServerStop(server);
	ServerDestroy(server);
	pthread_mutex_destroy(&mutex_log); // Destroy access switcher for log file
	pthread_mutex_destroy(&mutex_cmd); // Destroy access switcher for command line

	_fcloseall();
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
	server->status = stopped;


	// Bind the socket using the information in the sockaddr structure
	if (bind(server->socket, (SOCKADDR*)&server->address, sizeof(server->address)) == SOCKET_ERROR)
	{
		printf("Can't start server\n");
		free(server);
		exit(EXIT_FAILURE);
	}
	

	// Success
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

// Server thread
void* ServerRun(void* server_param)
{
	// Create a waiting line for 50 clients
	SERVER* server = (SERVER*)server_param;
	server->status = started;
	listen(server->socket, 50); 


	// Server startup time	
	char time_server_start[1000] = {0};
	strcat(time_server_start, "\n==============================================\n\n");
	strcat(time_server_start, "Server was started at ");
	char time_str_server_start[1000] = {0};
	const time_t time_raw_server_start = time(NULL);
	ctime_s(time_str_server_start, sizeof(time_str_server_start), &time_raw_server_start);
	strcat(time_server_start, time_str_server_start);
	strcat(time_server_start, "\n==============================================\n");
	LogWrite(time_server_start);


	// Listen for incoming connection requests on the server
	SOCKET client_socket;
	SOCKADDR_IN client_addr;
	int client_addr_size = sizeof(client_addr);
	while (1)
	{
		// Is it the time to close the server?
		pthread_testcancel();

		// Get client data
		client_socket = accept(server->socket, (SOCKADDR*)&client_addr, &client_addr_size);
		if (client_socket == INVALID_SOCKET)
		{
			//fprintf(log_file, "Error accept client\n");
			continue;
		}

		CLIENT* client = ClientCreate(client_socket, client_addr);
		pthread_create(&client->thread, NULL, ClientRun, (void*)client);
		pthread_detach(client->thread);
	}
}

void ServerStop(SERVER* server)
{
	if (pthread_cancel(server->thread))
		pthread_join(server->thread, NULL);
	server->status = stopped;
}

//=================================================================================

CLIENT* ClientCreate(SOCKET sock, SOCKADDR_IN sock_addr)
{
	CLIENT* client = (CLIENT*)malloc(sizeof(CLIENT));
	if (!client) exit(EXIT_FAILURE);

	client->socket = sock;
	client->address = sock_addr;

	return client;
}

void* ClientDestroy(CLIENT* client)
{
	closesocket(client->socket);
	free(client);
}

void* ClientRun(void* client_param)
{
	CLIENT* client = (CLIENT*)client_param;
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
