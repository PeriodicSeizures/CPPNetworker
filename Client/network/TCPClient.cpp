#include <stdio.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <queue>

#include "TCPClient.h"

TCPClient::TCPClient(asio::io_context &_io_context, std::string host, std::string port)
	: _socket(_io_context) {

	tcp::resolver resolver(_io_context);
	auto endpoints = resolver.resolve(host, port);

	do_connect(endpoints);
}

void TCPClient::do_connect(const tcp::resolver::results_type& endpoints) {
	asio::async_connect(_socket, endpoints,
		std::bind(&TCPClient::handle_read_header, 
			this, 
			std::placeholders::_1));
}

void TCPClient::handle_read_header(const asio::error_code& e) {

}