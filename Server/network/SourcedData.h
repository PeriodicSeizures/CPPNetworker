#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include "ConnectedClient.h"
#include "Packet.h"
struct SourcedData
{
	ConnectedClient *client;
	//Packet *packet; // instantiated packet
	//ByteReader *reader;
	char *allocated; // allocated with malloc

	~SourcedData();
};

