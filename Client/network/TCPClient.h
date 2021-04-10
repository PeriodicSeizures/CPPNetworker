#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include "network/Packet.h"

class TCPClient
{
private:
	std::thread task;

	bool listening;
	SOCKET sock;

public:
	TCPClient();

public:

	void beginListeningAsync(const char* ip, uint16_t port);

	void sendPacket(Packet* packet);

private:
	void doListen();

	Packet* processPacket(ByteReader& reader);
};

#endif