#ifndef CLIENT_H
#define CLIENT_H

#define MAX_DATA_SIZE 1024
#define MAX_LOGIN_SIZE 30
#define MAX_PASSW_SIZE 30

#pragma comment(lib, "Ws2_32.lib")

#include <winsock.h>
#include <stdlib.h>
#include <stdio.h>

enum client_status  { offline, online };
enum connection_status { disconnected, connected};

typedef struct st_client
{
	SOCKET socket;
	SOCKADDR_IN address;

	char* login;
	unsigned char status : 1;
	unsigned char connection : 1;
}CLIENT;

CLIENT* ClientCreate  ();
void    ClientDestroy (CLIENT* client);

void SendData(CLIENT* client, char* data);
void ReceiveData(CLIENT* client, char* data);

void TryConnect(CLIENT* client);
void Registration(CLIENT* client);
void SignIn(CLIENT* client);
void SignOut(CLIENT* client);

#endif