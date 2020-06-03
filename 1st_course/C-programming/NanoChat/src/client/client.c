#include "client.h"

int main()
{
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(1, 1), &wsd)) return 1;

	CLIENT* client = ClientCreate();

	// Main client's console cycle
	while (1)
	{
		printf("<client> ");
		char command[51] = { 0 };
		gets_s(command, 50);

		if (!strcmp(command, "help"))
		{
			printf("\n");
			printf("|     help - list of commands\n");
			printf("|      cls - clear screen\n");
			printf("|   status - show your status\n");
			printf("|  connect - try to connect with the server\n");
			printf("| register - try to connect with the server\n");
			printf("|   signin - enter in the system\n");
			printf("|  signout - exit the system\n");
			printf("|     exit - shutdown the program\n");
			printf("\n");
			continue;
		}

		if (!strcmp(command, "cls"))
		{
			system("cls");
			continue;
		}

		if (!strcmp(command, "connect"))
		{
			if (client->connection == disconnected)
				TryConnect(client);
			else
				printf("You already connected\n\n");
			continue;
		}

		if (!strcmp(command, "register"))
		{
			if (client->connection == disconnected)
			{
				printf("You disconnected to server. Use \"connect\" to fix it.\n\n");
				continue;
			}
			system("cls");
			Registration(client);
			continue;
		}

		if (!strcmp(command, "signin"))
		{
			if (client->connection == disconnected)
			{
				printf("You disconnected to server. Use \"connect\" to fix it.\n\n");
				continue;
			}
				
			system("cls");
			SignIn(client);
			continue;
		}


		if (!strcmp(command, "exit"))
			break;

		printf("Command not found. Use \"help\" to show commands\n\n");
	}

	ClientDestroy(client);

	WSACleanup(); // It terminates use of the winsock library (Ws2_32.dll).
	return 0;
}


CLIENT* ClientCreate()
{
	CLIENT* client = (CLIENT*)malloc(sizeof(CLIENT));
	if (!client) exit(EXIT_FAILURE);

	// Create a socket
	client->socket = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
	if (client->socket == INVALID_SOCKET)
	{
		printf("Error create socket\n");
		return;
	}

	// Set connection settings
	client->address.sin_family = AF_INET;
	client->address.sin_port = htons(5510); // server port
	client->address.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); // loopback | address of your own network adapter
	client->connection = disconnected;
	TryConnect(client);

	client->login = NULL;
	client->status = offline;

	return client;
}

void TryConnect(CLIENT* client)
{
	if (connect(client->socket, (SOCKADDR*)&(client->address), sizeof(client->address)) == SOCKET_ERROR)
		printf("Can't connect to server. Please, try later\n\n");
	else
	{
		client->connection = connected;
		printf("Connection was established\n\n");
	}	
}

void ClientDestroy(CLIENT* client)
{
	closesocket(client->socket);
	free(client);
}

void SendData(CLIENT* client, char* data)
{
	// Send the data to server
	int ret = send(client->socket, data, MAX_DATA_SIZE, 0);
	if (ret == SOCKET_ERROR)
		printf("Can't send message\n");
}

void ReceiveData(CLIENT* client, char* data)
{
	// Receive the answer from server
	memset(data, 0, MAX_DATA_SIZE);
	int ret = recv(client->socket, data, MAX_DATA_SIZE, 0);
	if (ret == 0 || ret == WSAECONNRESET)
		printf("Connection closed\n");
}

void Registration(CLIENT* client)
{
	char* data = (char*)calloc(MAX_DATA_SIZE + 1, sizeof(char));
	if (!data) exit(EXIT_FAILURE);

	char* login = (char*)calloc(MAX_LOGIN_SIZE + 1, sizeof(char));
	if (!login) exit(EXIT_FAILURE);

	char* passw = (char*)calloc(MAX_PASSW_SIZE + 1, sizeof(char));
	if (!passw) exit(EXIT_FAILURE);

	printf("Registration\n");
	strcat(data, "<__register__> login: ");
	printf("| login: ");
	strcat(data, gets_s(login, MAX_LOGIN_SIZE));

	strcat(data, " passw: ");
	printf("| password: ");
	strcat(data, gets_s(login, MAX_PASSW_SIZE));

	SendData(client, data);
	ReceiveData(client, data);

	printf("\n%s\n\n", data);

	free(data);
	free(login);
	free(passw);
}

void SignIn(CLIENT* client)
{
	char* data = (char*)calloc(MAX_DATA_SIZE + 1, sizeof(char));
	if (!data) exit(EXIT_FAILURE);

	char* login = (char*)calloc(MAX_LOGIN_SIZE + 1, sizeof(char));
	if (!login) exit(EXIT_FAILURE);

	char* passw = (char*)calloc(MAX_PASSW_SIZE + 1, sizeof(char));
	if (!passw) exit(EXIT_FAILURE);

	printf("Sign in\n");
	strcat(data, "<__signin__> login: ");
	printf("| login: ");
	strcat(data, gets_s(login, MAX_LOGIN_SIZE));

	strcat(data, " passw: ");
	printf("| password: ");
	strcat(data, gets_s(login, MAX_PASSW_SIZE));

	SendData(client, data);
	ReceiveData(client, data);

	printf("\n%s\n\n", data);

	free(data);
	free(login);
	free(passw);
}