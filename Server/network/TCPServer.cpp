#include <algorithm>
#include "TCPServer.h"

TCPServer::TCPServer() : listening(false), sock(0) {

}

void TCPServer::beginListeningAsync(uint16_t port) {

	if (listening) {
		printf("already listening!\n");
		return;
	}

    if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
    {
        printf("could not create tcp socket : %d\n", WSAGetLastError());
        //exit(EXIT_FAILURE);
    }

    sockaddr_in server;

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if (bind(this->sock, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
    {
        printf("tcp bind() failed with error code : %d\n", WSAGetLastError());
        //exit(EXIT_FAILURE);
    }

	//Listen to incoming connections
	listen(this->sock, 3);

	task = std::thread(&TCPServer::doListen, this);
}

static char data[512];
void TCPServer::sendPacket(Packet* packet, SOCKET socket) {
    ByteWriter writer(data);
    packet->write(writer);
    if (send(sock, data, writer.getOffset(), 0) == SOCKET_ERROR)
    {
        printf("tcp send() failed with error code : %d\n", WSAGetLastError());
        //exit(EXIT_FAILURE);
    }
}

void TCPServer::doListen() {

	this->listening = true;

	sockaddr_in address;
    int addrlen = sizeof(sockaddr_in);

	fd_set readfds;

	//SOCKET s;

	char recv_buf[512];

	//const int max_clients = 30;
	//SOCKET new_socket; // , client_socket[max_clients];

    while (listening) {

		//clear the socket fd set
		FD_ZERO(&readfds);

		//add master socket to fd set
		FD_SET(this->sock, &readfds);

		for (SOCKET s : clients) {
			if (s > 0)
				FD_SET(s, &readfds);
		}

		//wait for an activity on any of the sockets, timeout is NULL , so wait indefinitely
		int activity = select(0, &readfds, NULL, NULL, NULL);

		if (activity == SOCKET_ERROR)
		{
			printf("tcp select() failed with error code : %d\n", WSAGetLastError());
			//exit(EXIT_FAILURE);
		}

		//If something happened on the master socket , then its an incoming connection
		if (FD_ISSET(this->sock, &readfds))
		{
			SOCKET client_sock = accept(this->sock, (sockaddr*)&address, &addrlen);
			if (client_sock == SOCKET_ERROR)
			{
				printf("tcp accept() failed with error code : %d\n", WSAGetLastError());
				//exit(EXIT_FAILURE);
			}

			printf("client connect\n");

			clients.insert(client_sock);
		}

		//else its some IO operation on some other socket :)
		//for (int i = 0; i < max_clients; i++)
		//auto it = clients.begin();
		//std::set<SOCKET>::iterator it = clients.begin();
		//while (it != clients.end())
		for (auto it = clients.begin(); it != clients.end(); )
		{
			//SOCKET s = clients[i];
			//if client presend in read sockets
			SOCKET client_sock = *it;

			if (FD_ISSET(client_sock, &readfds))
			{
				//get details of the client
				getpeername(client_sock, (sockaddr*)&address, &addrlen);

				//Check if it was for closing , and also read the incoming message
				//recv does not place a null terminator at the end of the string (whilst printf %s assumes there is one).

				int recv_len = recv(client_sock, recv_buf, sizeof(recv_buf), 0);
				if (recv_len == SOCKET_ERROR)
				{
					int error_code = WSAGetLastError();
					if (error_code == WSAECONNRESET) {
						//disconnectSocket(client_sock);
						
						it = clients.erase(it);
						closesocket(client_sock);

					} else {
						printf("tcp recv() failed with error code : %d", error_code);
						//exit(0);
					}
				} 
				else if (recv_len == 0) {
					//disconnectSocket(client_sock);
					it = clients.erase(it);
					closesocket(client_sock);
				}
				else {
					// process the tcp packet

					++it;

					ByteReader reader(recv_buf);

					Packet *packet = processPacket(reader, client_sock);

					delete packet;

				}
			}
			else
				++it;
		}

    }

	//WSACleanup();

}

Packet *TCPServer::processPacket(ByteReader& reader, SOCKET client_sock) {

    //ByteReader reader(data->allocated);

    Packet* packet;

    switch (reader.readUnsignedShort()) {

		// all packets received come from client
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

void TCPServer::disconnectSocket(SOCKET client_sock) {//, std::set<SOCKET>::iterator it) {
	printf("Disconnected socket : %llu", client_sock);

	//it = clients.erase(client_sock);

	clients.erase(client_sock);

	closesocket(client_sock);
}