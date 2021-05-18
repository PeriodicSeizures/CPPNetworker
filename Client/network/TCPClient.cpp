#include <stdio.h>
#include <vector>
#include <sstream>
#include <fstream>
#include <string>
#include <queue>

#include "TCPClient.h"

TCPClient::TCPClient(std::string host, std::string port)
	: _socket(_io_context) {

	tcp::resolver resolver(_io_context);
	auto endpoints = resolver.resolve(host, port);

	do_connect(endpoints);
}

/*
void TCPClient::do_write(const asio::error_code& e) {






asio::async_write(_socket,
asio::buffer((void*)&(out_packet_queue.front()), Packet::HEADER_SIZE + Packet::sizes[),
[this](const asio::error_code& e)
{
if (!e) {
asio::async_write(_socket,
asio::buffer((void*)packet->type, Packet::HEADER_SIZE),
[this](const asio::error_code& e)
{
if (!e) {

}
else {
std::cout << "send_packet body error: " << e.message() << "\n";
_socket.close();
}
}
}
else {
std::cout << "send_packet header error: " << e.message() << "\n";
_socket.close();
}
}






asio::async_write(_socket,
asio::buffer((void*)packet->type, Packet::HEADER_SIZE),
[this](const asio::error_code &e)
{
if (!e) {
//*
//* start of body send
//*
asio::async_write(_socket,
asio::buffer((void*)packet->type, Packet::HEADER_SIZE),
[this](const asio::error_code& e)
{
if (!e) {

}
else {
std::cout << "send_packet body error: " << e.message() << "\n";
_socket.close();
}
}
}
else {
std::cout << "send_packet header error: " << e.message() << "\n";
_socket.close();
}
}
}
*/

void TCPClient::send_packet(Packet& packet) {
	out_packet_queue.push_back(packet);
}

void TCPClient::do_connect(const tcp::resolver::results_type& endpoints) {
	asio::async_connect(_socket, endpoints,
		std::bind(&TCPClient::handle_read_header,
			this,
			std::placeholders::_1));
}

void TCPClient::handle_read_header(const asio::error_code& e) {
	if (!e) {
		asio::async_read(_socket,
			asio::buffer((void*)in_packet_type, Packet::HEADER_SIZE),
			std::bind(&TCPClient::handle_read_body, this,
				std::placeholders::_1
			));
	}
	else {
		std::cout << "header error: " << e.message() << "\n";
	}
}

void TCPClient::handle_read_body(const asio::error_code& e) {

	if (!e) {

		Packet packet;
		packet.type = in_packet_type;

		in_packet_queue.push_back(packet);

		asio::async_read(_socket,
			asio::buffer(packet.data, Packet::sizes[(uint16_t)in_packet_type]),
			std::bind(&TCPClient::handle_read_header, this,
				std::placeholders::_1
			));
	}
	else {
		std::cout << "body error: " << e.message() << "\n";
	}
}

void TCPClient::handle_send_header(const asio::error_code& e) {
	if (!e) {
		// pop prev body packet
		out_packet_queue.pop_front();

		asio::async_write(_socket,
			asio::buffer((void*)(out_packet_queue.front().type), sizeof(Packet::Type)),
			std::bind(&TCPClient::handle_send_body, this, std::placeholders::_1)
		);
	}
	else {
		std::cout << "send_packet header error: " << e.message() << "\n";
		_socket.close();
	}
}

void TCPClient::handle_send_body(const asio::error_code& e) {
	if (!e) {
		asio::async_write(_socket,
			asio::buffer(out_packet_queue.front().data),
			std::bind(&TCPClient::handle_send_body, this, std::placeholders::_1)
		);
	}
	else {
		std::cout << "send_packet body error: " << e.message() << "\n";
		_socket.close();
	}
}