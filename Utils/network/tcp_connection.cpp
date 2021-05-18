#include <iostream>
#include "tcp_connection.h"

void TCPConnection::init_io_context(asio::io_context& _io_context) {
	TCPConnection::_io_context = &_io_context;
}

TCPConnection::pointer TCPConnection::create() {
	return pointer(new TCPConnection());
}

tcp::socket& TCPConnection::socket()
{
	return _socket;
}

void TCPConnection::start() {
	asio::error_code e; // dummy error
	handle_read_header(e);
}

bool TCPConnection::is_connected() {
	return _socket.is_open();
}

void TCPConnection::connect_to_server(std::string host, std::string port) {
	tcp::resolver resolver(*_io_context);
	auto endpoints = resolver.resolve(host, port);

	//do_connect(endpoints);
	asio::async_connect(_socket, endpoints,
		std::bind(&TCPConnection::handle_read_header,
			this,
			std::placeholders::_1));
}



TCPConnection::TCPConnection()
	: _socket(*_io_context)
{
	//std::hash<std::string> hasher;
	//uuid = hasher(socket_.remote_endpoint().address().to_string());
}

/*
* handle_read_header and handle_read_body are both consistent in terms of call order 
* header -> body -> header -> body -> header -> body etc...
* since this order is guaranteed, a thread dangerous queue could work, but will not suffice,
* as the server itself will iterate clients and retrieve packets asynchronously from each of their queues
*/

void TCPConnection::handle_read_header(const asio::error_code& e) {
	if (!e) {
		asio::async_read(_socket,
			asio::buffer((void*)in_packet_type, Packet::SIZE),
			std::bind(&TCPConnection::handle_read_body, shared_from_this(),
				std::placeholders::_1)
		);
	}
	else {
		std::cout << "r body error: " << e.message() << "\n";
		delete this;
	}
}

void TCPConnection::handle_read_body(const asio::error_code& e)
{
	if (!e) {
		std::cout << "incoming header: " << (uint16_t)in_packet_type << "\n";

		in_packets.push_back({ in_packet_type, new char[Packet::S(in_packet_type)] });

		asio::async_read(_socket,
			asio::buffer((void*)(in_packets.back().data), Packet::S(in_packet_type)),
			std::bind(&TCPConnection::handle_read_header, shared_from_this(),
				std::placeholders::_1)
		);
	}
	else {
		std::cout << "r header error: " << e.message() << "\n";
		delete this;
	}
}

void TCPConnection::handle_write_header(const asio::error_code& e)
{
	if (!e && !out_packets.empty()) {
		asio::async_write(_socket,
			asio::buffer((void*)(out_packets.front().type), Packet::SIZE),
			std::bind(&TCPConnection::handle_write_body, shared_from_this(),
				std::placeholders::_1)
		);
	}
	else {
		std::cout << "w header error: " << e.message() << "\n";
		delete this;
	}
}

void TCPConnection::handle_write_body(const asio::error_code& e)
{
	if (!e) {
		asio::async_write(_socket,
			asio::buffer((void*)(out_packets.front().data), Packet::S(out_packets.front().type)),
			std::bind(&TCPConnection::handle_write_body, shared_from_this(),
				std::placeholders::_1)
		);
	}
	else {
		std::cout << "w body error: " << e.message() << "\n";
		delete this;
	}
}

/*
* Static definitions
*/
asio::io_context *TCPConnection::_io_context;