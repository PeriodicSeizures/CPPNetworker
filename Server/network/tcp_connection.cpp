#include <iostream>
#include "tcp_connection.h"
#include "tcp_server.h"
#include "OwnedPacket.h"

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

	asio::error_code e; // dummy error
	handle_read_header(e);
}

bool TCPConnection::is_connected() {
	return socket_.is_open();
}

TCPConnection::TCPConnection(asio::io_context& io_context)
	: socket_(io_context) //, std::enable_shared_from_this<tcp_connection>() // TEST SUPER
{
	std::hash<std::string> hasher;
	uuid = hasher(socket_.remote_endpoint().address().to_string());
}

void TCPConnection::handle_write(const asio::error_code& e)
{
	// optional something to do after data is sent
	// ...
}


void TCPConnection::handle_read_header(const asio::error_code& e) {
	if (!e) {
		//std::cout << "incoming body: " << recv_buf << "\n";

		asio::async_read(socket_,
			asio::buffer((void*)in_packet_type, Packet::HEADER_SIZE),
			std::bind(&TCPConnection::handle_read_body, shared_from_this(),
				std::placeholders::_1)
		);
	}
	else {
		std::cout << "body error: " << e.message() << "\n";
		delete this;
	}
}


void TCPConnection::handle_read_body(const asio::error_code& e)
{
	// optional something to do after data is received
	// ...

	if (!e) {
		std::cout << "incoming header: " << (uint16_t)in_packet_type << "\n";

		// Prepare packet for queue
		OwnedPacket owned_packet;
		owned_packet.packet.type = in_packet_type;
		owned_packet.owner = uuid;
		
		main_server->incoming_packets.push_back(owned_packet);

		asio::async_read(socket_,
			asio::buffer(owned_packet.packet.data, Packet::sizes[(uint16_t)in_packet_type]),
			std::bind(&TCPConnection::handle_read_header, shared_from_this(),
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

TCPServer *TCPConnection::main_server;