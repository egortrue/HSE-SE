#include "server.h"

FILE* file_log;
FILE* file_database;
pthread_mutex_t mutex_cmd;
pthread_mutex_t mutex_log;
pthread_mutex_t mutex_db;

int main()
{
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(1, 1), &wsd)) return 1;		

	file_log      = FileOpen("..\\src\\server\\data\\log.txt", "a");
	file_database = FileOpen("..\\src\\server\\data\\database.txt", "a+");

	// Init the access switchers
	pthread_mutex_init(&mutex_cmd, NULL);
	pthread_mutex_init(&mutex_log, NULL); 
	pthread_mutex_init(&mutex_db,  NULL);

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
				printf("Server is started\n\n");
			else
				printf("Server is stopped\n\n");
			continue;
		}

		if (!strcmp(command, "start"))
		{
			if (server->status == stopped)
			{
				pthread_create(&server->thread, NULL, ServerRun, (void*)server);
				printf("Server is started\n\n");
			}
			else
				printf("Server already started\n\n");
			continue;
		}

		if (!strcmp(command, "stop"))
		{
			if (server->status == started)
			{
				ServerStop(server);
				printf("Server is stopped\n\n");
			}
			else
				printf("Server already stopped\n\n");
			continue;
		}

		if (!strcmp(command, "exit"))
			break;

		printf("Command not found. Use \"help\" to show commands\n\n");
	}

	ServerStop(server);
	ServerDestroy(server);

	// Destroy the access switchers
	pthread_mutex_destroy(&mutex_db);
	pthread_mutex_destroy(&mutex_log);
	pthread_mutex_destroy(&mutex_cmd);

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
	server->clients = (CLIENT**)calloc(MAX_CLIENTS_ONLINE, sizeof(CLIENT*));
	if (!server->clients) exit(EXIT_FAILURE);


	// Default options
	server->status = stopped;
	server->clients_online = 0;


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

	pthread_mutex_lock(&mutex_log);
	fprintf(file_log, time_server_start);
	pthread_mutex_unlock(&mutex_log);


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
			continue;

		// Create a client and start a thread for work with him
		CLIENT* client = ClientCreate(client_socket, client_addr);

		struct _datum { SERVER* server; CLIENT* client; };
		struct _datum* datum = (struct _datum*)malloc(sizeof(struct _datum));
		datum->client = client;
		datum->server = server;

		pthread_create(&client->thread, NULL, ClientRun, (void*)datum);
		pthread_detach(client->thread);
	}
	return NULL;
}

void ServerStop(SERVER* server)
{
	if (server->status == started)
	{
		if (pthread_cancel(server->thread))
			pthread_join(server->thread, NULL);
		server->status = stopped;
	}
}

//=================================================================================

CLIENT* ClientCreate(SOCKET sock, SOCKADDR_IN sock_addr)
{
	CLIENT* client = (CLIENT*)malloc(sizeof(CLIENT));
	if (!client) exit(EXIT_FAILURE);

	client->socket = sock;
	client->address = sock_addr;

	client->login = NULL; // Client not enter in the DeepWeb yet.

	return client;
}

void* ClientDestroy(CLIENT* client)
{
	closesocket(client->socket);
	if (client->login != NULL) 
		free(client->login);
	free(client);
	return NULL;
}

void* ClientRun(void* client_param_)
{
	struct _datum { SERVER* server; CLIENT* client; };
	struct _datum* client_param = (struct _datum*)client_param_;

	CLIENT* client = client_param->client;
	SERVER* server = client_param->server;

	free(client_param);

	// Main thread cycle
	while (1)
	{
		// If server is closed -> stop the thread
		if (server->status == stopped)
			break;

		// Receive the data from client
		// If we can't receive the data -> stop the thread
		char data_receive[MAX_DATA_SIZE] = { 0 };
		int data_size = recv(client->socket, data_receive, MAX_DATA_SIZE, 0);
		if (data_size <= 0)
			break;
			

		// Write down the log of receiving
		pthread_mutex_lock(&mutex_log);
		fprintf(file_log, "%s\n", data_receive);
		pthread_mutex_unlock(&mutex_log);


		// Register in the system (write down to the database)
		// Example: "<__register__> login: Egor passw: 12345"
		if (strstr(data_receive, "<__register__>"))
		{
			// Get login and password from received data
			char* login_pos = strstr(data_receive, "login: ") + 7;
			char* passw_pos = strstr(data_receive, "passw: ") + 7;
			int login_len = (int)passw_pos - (int)login_pos - 8;
			int passw_len = (int)strlen(passw_pos);

			char* login = (char*)calloc(login_len + 1, sizeof(char));
			if (!login) exit(EXIT_FAILURE);

			char* passw = (char*)calloc(passw_len + 1, sizeof(char));
			if (!passw) exit(EXIT_FAILURE);

			memcpy(login, login_pos, login_len);
			memcpy(passw, passw_pos, passw_len);


			// Find simillar data in the database
			char good[] = "You successfully registered";
			char  bad[] = "You already registered";

			pthread_mutex_lock(&mutex_db);
			char* user_db = FindUserInDataBase(login);
			if (user_db != NULL)
			{
				send(client->socket, bad, strlen(bad), 0);
				free(user_db);
			}	
			else
			{
				fseek(file_database, 0, SEEK_END);
				fprintf(file_database, "%s|%s\n", login, passw); // Register in the database
				send(client->socket, good, strlen(good), 0);
			}
			pthread_mutex_unlock(&mutex_db);

			// Clear all received data anyway
			free(login);
			free(passw);
			continue;
		}


		// Enter in the system
		// Example: "<__signin__> login: Captain_Jack passw: Black_Pearl"
		if (strstr(data_receive, "<__signin__>"))
		{
			// Get login and password from received data
			char* login_pos = strstr(data_receive, "login: ") + 7;
			char* passw_pos = strstr(data_receive, "passw: ") + 7;
			int login_len = (int)passw_pos - (int)login_pos - 8;
			int passw_len = (int)strlen(passw_pos);

			char* login = (char*)calloc(login_len + 1, sizeof(char));
			if (!login) exit(EXIT_FAILURE);

			char* passw = (char*)calloc(passw_len + 1, sizeof(char));
			if (!passw) exit(EXIT_FAILURE);

			memcpy(login, login_pos, login_len);
			memcpy(passw, passw_pos, passw_len);


			// Find simillar data in the database
			char good[] = "You successfully entered in the system!";
			char bad_reg[] = "You are not registered";
			char bad_pas[] = "Password is wrong";

			pthread_mutex_lock(&mutex_db);
			char* user_db = FindUserInDataBase(login);
			if (user_db != NULL)
			{

				// Get password from database
				char* db_passw = strchr(user_db, '|') + 1;
				char* db_passw_end = strchr(user_db, '\n');
				memset(db_passw_end, 0, 1);

				// Compare passwords
				if (strcmp(passw, db_passw))
				{
					send(client->socket, bad_pas, strlen(bad_pas), 0);
					free(login);
				}
				else
				{
					send(client->socket, good, strlen(good), 0);
					client->login = login; // Client entered in the DeepWeb
				}

				free(user_db);
			}
			else
			{
				send(client->socket, bad_reg, strlen(bad_reg), 0);
				free(login);
			}		
			pthread_mutex_unlock(&mutex_db);


			free(passw);
			continue;
		}


		// Exit the system
		if (strstr(data_receive, "<__signout__>"))
		{
			free(client->login);
			client->login = NULL;  // Client leave the DeepWeb
			return NULL;
		}

		// Send the message to current user
		if (strstr(data_receive, "<__message_current_user__>"))
		{

		}


		// Send the message to group of users
		if (strstr(data_receive, "<__message_group__>"))
		{

		}

	}

	// Stop the thread
	ClientDestroy(client);
	return NULL;
}

//=================================================================================

char* FindUserInDataBase(char* login)
{
	// Size of file
	fseek(file_database, 0, SEEK_END);
	int file_size = ftell(file_database);

	// Go to the begin of file
	fseek(file_database, 0, SEEK_SET);

	// Buffer for checking
	char* string = (char*)calloc(MAX_DATA_SIZE, sizeof(char));
	if (!string) exit(EXIT_FAILURE);

	// Address of "|" in string
	char* separator = NULL;

	do
	{

		// If the end of file
		if (ftell(file_database) == file_size)
		{
			free(string);
			return NULL;
		}

		// Clear the string which was before
		memset(string, 0, MAX_DATA_SIZE);

		// Read a string from file
		fgets(string, MAX_DATA_SIZE, file_database);
		
		// Find the address of separator
		separator = strchr(string, '|');
		if (separator == NULL)
		{
			free(string);
			return NULL;
		}

		// Set zero on separator's place for string compare
		memset(separator, 0, 1);

	} while (strcmp(login, string));

	// Restore the separator in return string
	memset(separator, '|', 1);
	return string;
}


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

//=================================================================================
