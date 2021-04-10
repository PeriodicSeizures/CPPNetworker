#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <iostream>
#include "ByteReader.h"
#include "UDPServer.h"
#include "Packet.h"

UDPServer::UDPServer() : listening(true) {
}

void UDPServer::beginListener(short port) {

	//SOCKET s;
	//sockaddr_in server, si_other;
	//int slen, recv_len;
	//char buf[BUFLEN];
	//WSADATA wsa;

	//slen = sizeof(si_other);

	//Initialise winsock
	printf("Initialised.\n");

	//Create a socket
	//UDP utilizes datagrams, whereas TCP utilizes a stream
	if ((this->sock = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET)
		printf("Could not create UDP socket : %d", WSAGetLastError());
	else
		printf("UDP Socket created.\n");

	sockaddr_in server;

	//Prepare the sockaddr_in structure
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	//Bind
	if (bind(this->sock, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		printf("UDP Bind failed with error code : %d", WSAGetLastError());
		WSACleanup();
		exit(EXIT_FAILURE);
	}
	puts("UDP Bind done");

	task = std::thread(&UDPServer::doListen, this);
}

static char data[512];

//void UDPNetworker::sendPacket(Packet *packet, sockaddr_in *to, int to_len) {
//	ByteWriter writer(data);
//	packet->write(writer);
//	sendto(sock, data, writer.getOffset(), 0, (sockaddr*)to, to_len);
//}

void UDPServer::sendPacket(Packet* packet, ConnectedUDPClient *client) {
	ByteWriter writer(data);
	packet->write(writer);
	if (sendto(sock, data, writer.getOffset(), 0, (sockaddr*)&client->addr, client->addr_len) == SOCKET_ERROR) {
		printf("UDP sendto() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
}

// manipulating queue could cause issues with network thread
//void UDPNetworker::processQueuedData() {
//	while (!dataQueue.empty()) {
//		SourcedData &data = dataQueue.front();
//
//		this->processPacket(&data);
//
//		dataQueue.pop();
//	}
//}


void UDPServer::doListen() {

	sockaddr_in incoming_socket;

	int recv_len, slen = sizeof(incoming_socket);
	char recv_buf[512];

	//keep listening for data
	while (this->listening) {
		// blocking call which waits for data from any
		if ((recv_len = recvfrom(sock, recv_buf, sizeof(recv_buf), 0, (sockaddr*)&incoming_socket, &slen)) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d\n", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		// add client to list
		ConnectedUDPClient *client = addClient(incoming_socket, slen);

		ByteReader reader(recv_buf);

		processPacket(reader, client);
	}

	closesocket(this->sock);
	WSACleanup();
}

Packet * UDPServer::processPacket(ByteReader &reader, const ConnectedUDPClient* client) {

	//ByteReader reader(data->allocated);

	Packet *packet;

	switch (reader.readUnsignedShort()) {

	case Packet::SRC_CLIENT_CHAT: // pass a packet somehow?
		packet = new PacketChat();
		packet->read(reader);

		// will print just for debugging
		// BTW chat on a udp comms is bad
		packet->print();

		return packet;
	default:
		return nullptr;
	}

}

//void UDPNetworker::onPacketReceive() {}

ConnectedUDPClient* UDPServer::addClient(sockaddr_in connected_addr, int slen) {

	unsigned long addr = connected_addr.sin_addr.s_addr;

	// find
	auto&& find = this->clients.find(addr);

	if (find == clients.end()) {
		ConnectedUDPClient *client = new ConnectedUDPClient{ connected_addr, slen };
		this->clients.insert({ addr, client });
		return client;
	}
	return find->second;
}