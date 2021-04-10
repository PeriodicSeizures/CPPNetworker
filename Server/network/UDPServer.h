#ifndef NETWORKER_H
#define NETWORKER_H

#include <thread>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <map>
#include <vector>
#include <queue>

#include "Packet.h"
#include "ByteReader.h"
#include "ConnectedClient.h"


//#define BUFLEN 512

class UDPServer
{
private:
	std::thread task;
	bool listening;

	/**
	*	Network stuff
	*/

	SOCKET sock;
	//char recv_buf[BUFLEN];

	std::map<long, ConnectedUDPClient*> clients;

	// add a queue to process packets? this is udp so this would be pointless
	// minus well use tcp for this..
	//std::queue<SourcedData> dataQueue;

public:
	UDPServer();

	void beginListener(short port);

	// a mutex/lock is only needed on concurrent writes
	//void sendPacket(Packet *packet, sockaddr_in* to, int to_len);
	void sendPacket(Packet* packet, ConnectedUDPClient* client);

	// this should be ran from main thread or a new thread
	// (not the network thread, since it is dedicated to listen)
	//void processQueuedData();

private:
	void doListen();
	//Packet *processPacket(SourcedData *data);
	Packet* processPacket(ByteReader &reader, const ConnectedUDPClient *client);

	//void onPacketReceive(Packet *packet);
	//void onAsyncReceive(const char* buf);

	ConnectedUDPClient *addClient(sockaddr_in connected_addr, int slen);
};

#endif