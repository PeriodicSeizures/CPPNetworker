/*
	Packet based UDP Server
*/

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <queue>
#include "network/UDPServer.h"
#include "network/TCPServer.h"
#include "network/ByteReader.h"
#include "network/ByteWriter.h"

#pragma comment(lib, "ws2_32.lib") //Winsock Library

int main()
{
	WSADATA wsa;
	printf("\nInitialising Winsock...\n");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d\n", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//UDPNetworker sv = UDPNetworker();
	//sv.beginListener(8888);

	TCPServer sv = TCPServer();
	sv.beginListeningAsync(8888);




	/*
	std::string line;

	const char* filename = "chat_to_send.txt";
	std::ifstream infile(filename);

	if (!infile.is_open())
		printf("couldn't find %s\n", filename);

	std::queue<std::string> chat_strings_to_send;

	while (std::getline(infile, line)) {
		chat_strings_to_send.push(line);
	}

	while (true) {


		PacketChat packet(chat_strings_to_send.front());
		chat_strings_to_send.pop();
		sv.sendPacket(&packet, );



	}
*/


	while (true) {
		Sleep(1000);
	}



	return 0;
}