//#define

#include <stdio.h>
#include <winsock2.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <queue>
#include "TCPClient.h"

TCPClient::TCPClient() : listening(false) {
	this->sock = 0;
}

void TCPClient::beginListeningAsync(const char* ip, uint16_t port) {

	if (listening) {
		printf("already listening!\n");
		return;
	}

	

	//Create a socket
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Could not create socket : %d", WSAGetLastError());
	}

	printf("Socket created.\n");

	//unsigned long temp;
	//inet_pton(AF_INET, ip, &temp);

	sockaddr_in server;

	inet_pton(AF_INET, ip, &server.sin_addr.s_addr);
	//server.sin_addr.s_addr = temp; // inet_addr(ip);
	server.sin_family = AF_INET;
	server.sin_port = htons(port);

	//Connect to remote server
	if (connect(sock, (sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
	{
		puts("connect error");
		exit(1);
	}

	puts("Connected");


	task = std::thread(&TCPClient::doListen, this);
}

static char data[512];
void TCPClient::sendPacket(Packet* packet) {

	if (!listening)
		return;

	ByteWriter writer(data);
	packet->write(writer);

	if (send(sock, data, writer.getOffset(), 0) == SOCKET_ERROR)
	{
		printf("send failed");
		exit(EXIT_FAILURE);
	}
}

void TCPClient::doListen() {

	this->listening = true;

	char recv_buf[512];

	while (listening) {


		//Receive a reply from the server
		int recv_len = recv(sock, recv_buf, sizeof(recv_buf), 0);
		if (recv_len == SOCKET_ERROR)
		{
			printf("tcp recv() failed with error code : %d\n", WSAGetLastError());
			exit(EXIT_FAILURE);
		}
		else if (recv_len == 0) {
			printf("received a 0 length packet (disconnect?)");
		}
		else {
			ByteReader reader(recv_buf);

			delete processPacket(reader);
		}
				

	}
}

Packet* TCPClient::processPacket(ByteReader& reader) {

	//ByteReader reader(data->allocated);

	Packet* packet;

	switch (reader.readUnsignedShort()) {

	case Packet::SRC_SERVER_CHAT: // pass a packet somehow?
		packet = new PacketChat();
		packet->read(reader);

		// will print just for debugging
		packet->print();

		return packet;
	default:
		return nullptr;
	}

}