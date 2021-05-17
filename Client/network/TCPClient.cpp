//#define

#include <stdio.h>
//#include <winsock2.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <queue>

#ifdef _WIN32
#define _WIN32_WINNT 0xA00
#endif
#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>

#include "TCPClient.h"

TCPClient::TCPClient() : listening(false) {
	this->sock = 0;
}

TCPClient::~TCPClient() {
	listening = false;
	task.join();
	closesocket(sock);
}

void TCPClient::beginListeningAsync(const char* ip, uint16_t port) {

	if (listening) {
		printf("already listening!\n");
		return;
	}

	//task = std::thread(&TCPClient::doListen, this);
	task = std::thread([](TCPClient *instance, const char* ip, uint16_t port) {
		
		asio::error_code ec;
		asio::io_context context;

		asio::ip::tcp::endpoint endpoint(asio::ip::make_address(ip, ec), port);

		instance->doListen();

	}, this, ip, port);
}

static char data[512];
void TCPClient::sendPacket(Packet* packet) {

	if (!listening)
		return;

	ByteWriter writer(data);
	packet->write(writer);

	if (send(sock, data, writer.getOffset(), 0) == SOCKET_ERROR)
	{
		printf("tcp send() failed with error code : %d\n", WSAGetLastError());
		//exit(EXIT_FAILURE);
	}
}

void TCPClient::doListen() {

	

	

	char recv_buf[512];

	while (listening) {


		//Receive a reply from the server
		int recv_len = recv(sock, recv_buf, sizeof(recv_buf), 0);
		if (recv_len == SOCKET_ERROR)
		{
			int error_code = WSAGetLastError();
			if (error_code == WSAECONNRESET) {
				printf("server was closed\n");
				close();
			}
			else {
				// 10054 on unexpected server close
				printf("tcp recv() failed with error code : %d\n", WSAGetLastError());
				Sleep(1000);
			}
			//exit(EXIT_FAILURE);
		}
		else if (recv_len == 0) {
			// a zero length packet could mean that conenction
			// was unexpectedly interrupted
			// or the transmissions are slow, so a timeout

			printf("received a 0 length packet (disconnect?)\n");
			close();
			
			//Sleep(1000);
		}
		else {
			ByteReader reader(recv_buf);

			delete deserialize(reader);
		}
				

	}

	

}

Packet* TCPClient::deserialize(ByteReader& reader) {

	//ByteReader reader(data->allocated);

	Packet* packet = Packet::deserialize(reader);

	if (packet != nullptr)
		packet->print();

	return packet;

}

void TCPClient::close() {

	closesocket(sock);
	listening = false;
	//WSACleanup();

}