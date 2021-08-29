#include "client.h"

int main()
{
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(1, 1), &wsd)) return 1;

	CLIENT* client = ClientCreate();

	// Main client's console cycle
	while (1)
	{
		if (client->status == offline)
			printf("<client> ");
		else
			printf("<client: %s> ", client->login);
		char command[51] = { 0 };
		gets_s(command, 50);

		if (!strcmp(command, "help"))
		{
			printf("\n");
			printf("|     help - list of commands\n");
			printf("|      cls - clear screen\n");
			printf("|   status - show your status\n|\n");

			printf("|  connect - try to connect with the server\n");
			printf("| register - register in the system\n|\n");

			printf("|   signin - enter in the system\n");
			printf("|  signout - exit the system\n|\n");

			printf("|   groups - list of groups\n");
			printf("|  chgroup - change your group id (1-128) (0 - without group)\n|\n");

			printf("|     send - send message to current user\n");
			printf("|   sendgr - send message to group\n|\n");

			printf("|      get - get messages which were sent to you\n");
			printf("|    getgr - get messages which were sent to your group\n|\n");

			printf("|     exit - shutdown the program\n");
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
			printf("\nStatus:\n");
			if (client->connection == connected)
				printf("| connected to server\n");
			else
			{
				printf("| disconnected to server\n\n");
				continue;
			}

			if (client->status == online)
				printf("| status: online\n");
			else
			{
				printf("| status: offline\n\n");
				continue;
			}

			printf("| login: %s\n", client->login);
			printf("| group id: %d\n\n", client->group_id);
			continue;
		}

		if (!strcmp(command, "connect"))
		{
			if (client->connection == connected)
			{
				printf("You already connected.\n\n");
				continue;
			}
			TryConnect(client);
			continue;
		}

		if (!strcmp(command, "register"))
		{
			if (client->connection == disconnected)
			{
				printf("You disconnected to server. Use \"connect\" to fix it.\n\n");
				continue;
			}

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

			if (client->status == online)
			{
				printf("You are sign in already. Use \"signout\" to leave the system.\n\n");
				continue;
			}
				
			SignIn(client);
			continue;
		}

		if (!strcmp(command, "signout"))
		{
			if (client->connection == disconnected)
			{
				printf("You disconnected to server. Use \"connect\" to fix it.\n\n");
				continue;
			}

			if (client->status == offline)
			{
				printf("You are not enter in the system yet. Use command \"signin\".\n\n");
				continue;
			}

			SignOut(client);
			continue;
		}


		if (!strcmp(command, "groups"))
		{
			if (client->connection == disconnected)
			{
				printf("You disconnected to server. Use \"connect\" to fix it.\n\n");
				continue;
			}

			Groups(client);
			continue;
		}


		if (!strcmp(command, "chgroup"))
		{
			if (client->connection == disconnected)
			{
				printf("You disconnected to server. Use \"connect\" to fix it.\n\n");
				continue;
			}

			if (client->status == offline)
			{
				printf("You are not enter in the system yet. Use command \"signin\".\n\n");
				continue;
			}

			ChangeGroup(client);
			continue;
		}



		if (!strcmp(command, "send"))
		{
			if (client->connection == disconnected)
			{
				printf("You disconnected to server. Use \"connect\" to fix it.\n\n");
				continue;
			}

			if (client->status == offline)
			{
				printf("You are not enter in the system yet. Use command \"signin\".\n\n");
				continue;
			}

			SendMessageToCurrentUser(client);
			continue;
		}


		if (!strcmp(command, "sendgr"))
		{
			if (client->connection == disconnected)
			{
				printf("You disconnected to server. Use \"connect\" to fix it.\n\n");
				continue;
			}

			if (client->status == offline)
			{
				printf("You are not enter in the system yet. Use command \"signin\".\n\n");
				continue;
			}

			SendMessageToGroup(client);
			continue;
		}


		if (!strcmp(command, "get"))
		{
			if (client->connection == disconnected)
			{
				printf("You disconnected to server. Use \"connect\" to fix it.\n\n");
				continue;
			}

			if (client->status == offline)
			{
				printf("You are not enter in the system yet. Use command \"signin\".\n\n");
				continue;
			}

			GetMessages(client);
			continue;
		}


		if (!strcmp(command, "getgr"))
		{
			if (client->connection == disconnected)
			{
				printf("You disconnected to server. Use \"connect\" to fix it.\n\n");
				continue;
			}

			if (client->status == offline)
			{
				printf("You are not enter in the system yet. Use command \"signin\".\n\n");
				continue;
			}

			GetGroupMessages(client);
			continue;
		}


		if (!strcmp(command, "exit"))
			break;

		printf("Command not found. Use \"help\" to show commands.\n\n");
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
		return NULL;
	}

	// Set connection settings
	client->address.sin_family = AF_INET;
	client->address.sin_port = htons(5510); // server port
	client->address.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); // loopback | address of your own network adapter
	client->connection = disconnected;
	TryConnect(client);

	client->login = NULL;
	client->status = offline;
	client->group_id = 0;

	return client;
}

void TryConnect(CLIENT* client)
{
	if (connect(client->socket, (SOCKADDR*)&(client->address), sizeof(client->address)) == SOCKET_ERROR)
		printf("Can't connect to server. Please, try later.\n\n");
	else
	{
		client->connection = connected;
		printf("Connection was established.\n\n");
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
	if (ret <= 0 || ret == WSAECONNRESET)
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

	printf("\nRegistration\n");
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

	printf("\nSign in\n");
	strcat(data, "<__signin__> login: ");
	printf("| login: ");
	strcat(data, gets_s(login, MAX_LOGIN_SIZE));

	strcat(data, " passw: ");
	printf("| password: ");
	strcat(data, gets_s(passw, MAX_PASSW_SIZE));

	SendData(client, data);
	ReceiveData(client, data);

	printf("\n%s\n\n", data);

	if (!strcmp(data, "You successfully entered in the system!"))
	{
		client->status = online;
		client->login = login;
		free(data);
		free(passw);
	}
	else
	{
		client->status = offline;
		client->login = NULL;
		free(login);
		free(data);
		free(passw);
	}
}

void SignOut(CLIENT* client)
{
	char* data = (char*)calloc(MAX_DATA_SIZE + 1, sizeof(char));
	if (!data) exit(EXIT_FAILURE);
	strcat(data, "<__signout__>");
	SendData(client, data);

	client->status = offline;
	free(client->login);
	client->login = NULL;
}

void Groups(CLIENT* client)
{
	char* data = (char*)calloc(MAX_DATA_SIZE + 1, sizeof(char));
	if (!data) exit(EXIT_FAILURE);
	strcat(data, "<__get_groups__>");

	SendData(client, data);
	ReceiveData(client, data);

	printf("%s\n", data);
	free(data);
}

void ChangeGroup(CLIENT* client)
{
	char* data = (char*)calloc(MAX_DATA_SIZE + 1, sizeof(char));
	if (!data) exit(EXIT_FAILURE);
	strcat(data, "<__set_group__> id: ");

	char* id_str = (char*)calloc(5, sizeof(char));
	if (!id_str) exit(EXIT_FAILURE);

	printf("Change group id (1-128): ");
	strcat(data, gets_s(id_str, 4));
	printf("\n");

	SendData(client, data);
	client->group_id = ConvertStrToInt(id_str);

	free(id_str);
}


void SendMessageToCurrentUser(CLIENT* client)
{
	char* data = (char*)calloc(MAX_DATA_SIZE + 1, sizeof(char));
	if (!data) exit(EXIT_FAILURE);

	strcat(data, "<__message_current_user__> to: ");

	char* recv_login = (char*)calloc(MAX_LOGIN_SIZE + 1, sizeof(char));
	if (!recv_login) exit(EXIT_FAILURE);

	printf("\nSend message to user\n");
	printf("| receiver: ");
	strcat(data, gets_s(recv_login, MAX_LOGIN_SIZE));

	char* message = (char*)calloc(MAX_MESS_SIZE + 1, sizeof(char));
	if (!message) exit(EXIT_FAILURE);

	strcat(data, " mes: ");
	printf("| message: ");
	strcat(data, gets_s(message, MAX_MESS_SIZE));

	SendData(client, data);
	ReceiveData(client, data);

	printf("\n%s\n\n", data);

	free(recv_login);
	free(message);
	free(data);
}

void SendMessageToGroup(CLIENT* client)
{
	char* data = (char*)calloc(MAX_DATA_SIZE + 1, sizeof(char));
	if (!data) exit(EXIT_FAILURE);

	strcat(data, "<__message_group__> id: ");

	char id[4] = "";
	strcat(data, itoa(client->group_id, &id, 10));

	printf("\nSend message to group #%d\n", client->group_id);

	char* message = (char*)calloc(MAX_MESS_SIZE + 1, sizeof(char));
	if (!message) exit(EXIT_FAILURE);

	strcat(data, " mes: ");
	printf("| message: ");
	strcat(data, gets_s(message, MAX_MESS_SIZE));

	SendData(client, data);
	ReceiveData(client, data);

	printf("\n%s\n\n", data);

	free(message);
	free(data);
}


void GetMessages(CLIENT* client)
{
	char* data = (char*)calloc(MAX_DATA_SIZE + 1, sizeof(char));
	if (!data) exit(EXIT_FAILURE);
	strcat(data, "<__get_messages__>");

	SendData(client, data);
	ReceiveData(client, data);

	char* message = strtok(data, "|\n");
	while (message)
	{
		message = strtok(NULL, "|\n");
		printf("%s\n", message);
		message = strtok(NULL, "|\n");
	}

	printf("\n");
	free(data);
}

void GetGroupMessages(CLIENT* client)
{
	char* data = (char*)calloc(MAX_DATA_SIZE + 1, sizeof(char));
	if (!data) exit(EXIT_FAILURE);
	strcat(data, "<__get_group_messages__>");

	SendData(client, data);
	ReceiveData(client, data);

	char* message = strtok(data, "|\n");
	while (message)
	{
		message = strtok(NULL, "|\n");
		printf("%s\n", message);
		message = strtok(NULL, "|\n");
	}

	printf("\n");
	free(data);
}


int ConvertStrToInt(char* string)
{
	int N = 0, i = 0, c = 1;
	if (*string == 45)
	{
		c = -1;
		string++;
	}
	while ((int)string[i])
		N = N * 10 + (int)string[i++] - 48;
	return N * c;
}
