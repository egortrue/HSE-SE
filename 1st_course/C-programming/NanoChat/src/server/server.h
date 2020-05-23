#ifndef SERVER_H
#define SERVER_H

#pragma comment(lib, "Ws2_32.lib")

#define HAVE_STRUCT_TIMESPEC
#define MAX_DATA_SIZE 1024
#define MAX_CLIENTS_ONLINE 50

#include <pthread.h>
#include <winsock.h>
#include <stdlib.h>
#include <stdio.h>

enum server_status {closed, started};
enum client_status {offline, online};

//===========================================================

typedef struct st_client
{
	SOCKET socket;
	SOCKADDR_IN address;
	time_t time_connect;
	int id;
	char* name;
}CLIENT;


void* ClientRun(void* client_param);

//===========================================================

typedef struct st_server
{
	// Server options
	SOCKET socket;
	SOCKADDR_IN address;
	pthread_t thread;
	time_t time_start;
	unsigned char status : 1;

	// Server data
	CLIENT** clients;

}SERVER;

SERVER* ServerCreate  ();
  void* ServerDestroy (SERVER* server);
  void* ServerRun     (void* server_param); // Create thread
  void  ServerStop    (SERVER* server);     // Close thread


//===========================================================
// Data functions
FILE* FileOpen(const char* name, const char* mode);
void LogWrite(char* string);


#endif