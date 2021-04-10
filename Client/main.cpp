/*
	Simple udp client
*/
#include <stdio.h>
#include <winsock2.h>
#include <iostream>
#include <Ws2tcpip.h>
#include <tchar.h>
#include <sstream>
#include <fstream>
#include <string>
#include <queue>
#include "network/TCPClient.h"

#pragma comment(lib,"ws2_32.lib") //Winsock Library

#define SERVER "127.0.0.1"	//ip address of udp server
#define BUFLEN 512	//Max length of buffer
#define PORT 8888	//The port on which to listen for incoming data

int main(void)
{
	WSADATA wsa;

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	TCPClient tcp_client;
	tcp_client.beginListeningAsync("192.168.1.200", 8888);







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

		if (chat_strings_to_send.empty())
			break;

		printf("delaying ... (3s)\n");

		Sleep(3000);

		PacketChat packet(chat_strings_to_send.front());
		chat_strings_to_send.pop();
		tcp_client.sendPacket(&packet);

	}

	while (true) {
		Sleep(1000);
	}


	return 0;





	struct sockaddr_in si_other;
	int s, slen = sizeof(si_other);
	char buf[BUFLEN];
	char message[BUFLEN];
	//WSADATA wsa;

	//Initialise winsock
	printf("\nInitialising Winsock...");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}
	printf("Initialised.\n");

	//create socket
	if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
	{
		printf("socket() failed with error code : %d", WSAGetLastError());
		exit(EXIT_FAILURE);
	}

	//setup address structure
	memset((char*)&si_other, 0, sizeof(si_other));
	si_other.sin_family = AF_INET;
	si_other.sin_port = htons(PORT);
//	si_other.sin_addr.S_un.S_addr = inet_addr(SERVER);
	// turns the string ip into a numeric ip
	InetPton(AF_INET, _T(SERVER), &si_other.sin_addr.S_un.S_addr);

	//start communication
	while (1)
	{
		printf("Enter message : ");

		std::cin >> message;
		message[0] = 0x1B; // -128;
		message[1] = 0x39;

		//message[0] = 0x40;
		//message[1] = 0x49;
		//message[2] = 0x0f;
		//message[3] = 0xdb;

		message[0] = (0x0);
		message[1] = (0x0); // chat packet








		//send the message
		if (sendto(s, message, 8, 0, (struct sockaddr*)&si_other, slen) == SOCKET_ERROR)
		{
			printf("sendto() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		//receive a reply and print it
		//clear the buffer by filling null, it might have previously received data
		memset(buf, '\0', BUFLEN);
		//try to receive some data, this is a blocking call
		if (recvfrom(s, buf, BUFLEN, 0, (struct sockaddr*)&si_other, &slen) == SOCKET_ERROR)
		{
			printf("recvfrom() failed with error code : %d", WSAGetLastError());
			exit(EXIT_FAILURE);
		}

		puts(buf);
	}

	closesocket(s);
	WSACleanup();

	return 0;
}