#include <iostream>
#include "tcp_connection.h"
#include "tcp_server.h"

TCPConnection::pointer TCPConnection::create(asio::io_context& io_context) {
	return pointer(new TCPConnection(io_context));
}

tcp::socket& TCPConnection::socket()
{
	return socket_;
}

void TCPConnection::start() {
	//message_ = make_daytime_string();
	//asio::async_write(socket_, asio::buffer(message_),
	//	std::bind(&TCPConnection::handle_write, shared_from_this(),
	//		std::placeholders::_1 // placeholder for error
	//		));

	asio::async_read(socket_, 
		asio::buffer((void*)in_packet_type, HEADER_SIZE),
		std::bind(&TCPConnection::handle_read_header, shared_from_this(),
			std::placeholders::_1 // placeholder for error
			));

}

bool TCPConnection::is_connected() {
	return socket_.is_open();
}

TCPConnection::TCPConnection(asio::io_context& io_context)
	: socket_(io_context)//, std::enable_shared_from_this<tcp_connection>() // TEST SUPER
{

}

void TCPConnection::handle_write(const asio::error_code& e)
{
	// optional something to do after data is sent
	// ...
}

void TCPConnection::handle_read_header(const asio::error_code& e)
{
	// optional something to do after data is received
	// ...

	if (!e) {
		std::cout << "incoming header: " << (uint16_t)in_packet_type << "\n";

		// Prepare packet for queue
		OwnedPacket* owned_packet = new OwnedPacket();
		main_server->incoming_packets.push_back(owned_packet);

		owned_packet->type = in_packet_type;

		asio::async_read(socket_,
			asio::buffer(owned_packet->data, Packet::sizes[(uint16_t)in_packet_type]),
			std::bind(&TCPConnection::handle_read_body, shared_from_this(),
				std::placeholders::_1 // placeholder for error
			));
	}
	else {
		// this should automatically kill the async read process,
		// but need to somehow kill the connection
		std::cout << "header error: " << e.message() << "\n";
		delete this; // kills connection
	}
}

void TCPConnection::handle_read_body(const asio::error_code& e) {
	if (!e) {
		//std::cout << "incoming body: " << recv_buf << "\n";

		asio::async_read(socket_,
			asio::buffer((void*)in_packet_type, HEADER_SIZE),
			std::bind(&TCPConnection::handle_read_header, shared_from_this(),
				std::placeholders::_1 // placeholder for error
			));
	}
	else {
		std::cout << "body error: " << e.message() << "\n";
		delete this;
	}
}

TCPServer *TCPConnection::main_server;