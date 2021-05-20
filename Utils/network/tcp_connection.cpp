#include <iostream>
#include "tcp_connection.h"

TCPConnection::pointer TCPConnection::create(asio::io_context& _io_context) {
	return pointer(new TCPConnection(_io_context));
}

tcp::socket& TCPConnection::socket() {
	return _socket;
}

void TCPConnection::start_reading() {
	read_header();
}

bool TCPConnection::is_connected() {
	return _socket.is_open();
}

void TCPConnection::connect_to_server(asio::io_context& _io_context, std::string host, std::string port) {
	tcp::resolver resolver(_io_context);
	auto endpoints = resolver.resolve(tcp::v4(), host, port);

	asio::async_connect(_socket, endpoints,
		std::bind(&TCPConnection::read_header, this));
}

TCPConnection::TCPConnection(asio::io_context &_io_context)
	: _socket(_io_context)
{
	//std::hash<std::string> hasher;
	//uuid = hasher(socket_.remote_endpoint().address().to_string());
}

void TCPConnection::read_header() {
	asio::async_read(_socket,
		asio::buffer((void*)in_packet_type, Packet::SIZE),
		[this](const std::error_code &e, size_t) {
		if (!e) {
			read_body();
		}
		else {
			std::cout << "read body error: " << e.message() << "\n";
			delete this;
		}
	}
	);
}

void TCPConnection::read_body() {
	in_packets.push_back({ in_packet_type, new char[Packet::S(in_packet_type)] });
	
	asio::async_read(_socket,
		asio::buffer((void*)(in_packets.back().data), Packet::S(in_packet_type)),
		[this](const std::error_code& e, size_t) {
		if (!e) {
			std::cout << "incoming header: " << (uint16_t)in_packet_type << "\n";
			read_header();
		}
		else {
			std::cout << "read header error: " << e.message() << "\n";
			delete this;
		}
	}
	);
}

void TCPConnection::write_header() {
	if (!out_packets.empty()) {
		asio::async_write(_socket,
			asio::buffer((void*)(out_packets.front().type), Packet::SIZE),
			[this](const std::error_code& e, size_t) {
			if (!e) {
				write_body();
			}
			else {
				std::cout << "write header error: " << e.message() << "\n";
				delete this;
			}
		}
		);
	}
}

void TCPConnection::write_body() {
	asio::async_write(_socket,
		asio::buffer((void*)(out_packets.front().data), Packet::S(out_packets.front().type)),
		[this](const std::error_code& e, size_t) {
		if (!e) {
			write_header();
		}
		else {
			std::cout << "write body error: " << e.message() << "\n";
			delete this;
		}
	}
	);
}
