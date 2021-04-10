#ifndef TCP_NETWORKER_H
#define TCP_NETWORKER_H

#include <thread>
#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <map>
#include <vector>
#include <queue>
#include <set>

#include "network/Packet.h"
#include "ConnectedClient.h"

class TCPServer
{

private:
	std::thread task;
	bool listening;

	/**
	*	Network stuff
	*/

	SOCKET sock;
	//char recv_buf[BUFLEN];

	//const static int max_clients = 30;
	//SOCKET clients[max_clients];

	std::set<SOCKET> clients;

public:
	TCPServer();

	void beginListeningAsync(uint16_t port);

	// a mutex/lock is only needed on concurrent writes
	//void sendPacket(Packet* packet, sockaddr_in* to, int to_len);
	void sendPacket(Packet* packet, SOCKET socket);

	// this should be ran from main thread or a new thread
	// (not the network thread, since it is dedicated to listen)
	//void processQueuedData();

private:
	void doListen();
	Packet* processPacket(ByteReader& reader, SOCKET client_sock);

	void disconnectSocket(SOCKET client_sock); // , std::set<SOCKET>::iterator it);
};

#endif