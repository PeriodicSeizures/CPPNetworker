#include <stdio.h>
#include <iostream>
#include <tchar.h>
#include <sstream>
#include <fstream>
#include <string>
#include <queue>
#include <asio.hpp>
#include "chat/InputTextHandler.h"
//#include <SDL_image.h>
#include "GlobalContainer.h"
#include "engine/Engine.h"
#include "engine/SpriteEngine.h"
#include "sprite/Player.h"

#include "network/TCPClient.h"

//#ifdef _WIN32
//#define _WIN32_WINNT 0xA00
//#endif
//#define ASIO_STANDALONE
//#include <asio.hpp>
//#include <asio/ts/buffer.hpp>
//#include <asio/ts/internet.hpp>

#define SERVER "127.0.0.1"	//ip address of udp server
#define BUFLEN 512	//Max length of buffer
#define PORT 8888	//The port on which to listen for incoming data

int main(void)
{
	Engine::init();
	SpriteEngine::init();

	try {
		asio::io_context io_context;
		TCPServer server(io_context);

		TCPConnection::main_server = &server;

		io_context.run();
	}
	catch (std::exception& e)
	{
		std::cout << "error: " << e.what() << "\n";
	}


	//std::thread tcp_task = std::thread(&TCPClient::beginListening, &tcp_client);

	GlobalContainer::tcp_client.beginListeningAsync("192.168.1.200", 8888);







	//std::string line;
	//
	//const char* filename = "chat_to_send.txt";
	//std::ifstream infile(filename);
	//
	//if (!infile.is_open())
	//	printf("couldn't find %s\n", filename);
	//
	//std::queue<std::string> chat_strings_to_send;
	//
	//while (std::getline(infile, line)) {
	//	chat_strings_to_send.push(line);
	//}
	//
	//while (!chat_strings_to_send.empty()) {
	//	PacketChat packet(chat_strings_to_send.front());
	//	chat_strings_to_send.pop();
	//	tcp_client.sendPacket(&packet);
	//
	//}

	//Engine::drawString("Hello world", 400, 300, 8, true);

	//SDL_StartTextInput();

	Player player;

	bool alive = true;
	bool render = true;
	while (alive) {

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			bool chatting = GlobalContainer::input_text_handler.processInput(e);

			switch (e.type)
			{
			case SDL_QUIT:
				alive = false;
				break;
			case SDL_WINDOWEVENT:
				switch (e.window.event) {
				case SDL_WINDOWEVENT_SHOWN:
				case SDL_WINDOWEVENT_EXPOSED:
				case SDL_WINDOWEVENT_MAXIMIZED:
				//case SDL_WINDOWEVENT_ENTER:
				case SDL_WINDOWEVENT_FOCUS_GAINED:
				//case SDL_WINDOWEVENT_TAKE_FOCUS:
					render = true;
					break;
				case SDL_WINDOWEVENT_HIDDEN:
				case SDL_WINDOWEVENT_MINIMIZED:
				//case SDL_WINDOWEVENT_LEAVE:
				case SDL_WINDOWEVENT_FOCUS_LOST:
					render = false;
					break;
				}
			case SDL_KEYDOWN:
				if (!chatting) {
					// game events
					break;
				}
			}
			
		}

		// must put all rendering in here or gpu will fall behind on rendering
		// when frames are dispatched due to all the queued frames
		if (render) {
			Engine::fill({0, 0, 0, 255});

			GlobalContainer::input_text_handler.render();

			player.render();

			Engine::doRender();
		}
		//Sleep(1000);

		//Engine::
	}

	//SDL_StopTextInput();

	//GlobalContainer::tcp_client.close();

	SpriteEngine::uninit();
	Engine::uninit();



	return 0;


	/*
		udp stuff
		*/

	/*
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
	*/
}