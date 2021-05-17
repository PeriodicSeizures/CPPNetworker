/*
	Packet based UDP Server
*/

#include <stdio.h>
#include <iostream>
#include <string>
#include "network/tcp_connection.h"
#include "network/tcp_server.h"

int main()
{
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

	return 0;
}