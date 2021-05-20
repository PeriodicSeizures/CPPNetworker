/*
	Packet based UDP Server
*/

#include <stdio.h>
#include <iostream>
#include <string>
//#include ""
#include "network/tcp_connection.h"
#include "network/tcp_server.h"

int main()
{
	try {
		TCPServer server(13);
	}
	catch (std::exception& e)
	{
		std::cout << "error: " << e.what() << "\n";
	}

	return 0;
}