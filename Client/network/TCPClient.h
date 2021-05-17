#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#ifdef _WIN32
#define _WIN32_WINNT 0xA00
#endif
#define ASIO_STANDALONE
#include <asio.hpp>

using namespace asio::ip;

#include "network/Packet.h"

class TCPClient
{
private:
	tcp::socket _socket;

public:
	TCPClient(asio::io_context& _io_context, 
		std::string host, std::string port);

private:
	void do_connect(const tcp::resolver::results_type& endpoints);

	void handle_read_header(const asio::error_code& e);
};

#endif