#pragma once
#include <asio.hpp>
#include <memory>
#include <deque>
#include <unordered_map>
#include "network/Packet.h"

class TCPServer;

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE

using namespace asio::ip;

class TCPConnection : public std::enable_shared_from_this<TCPConnection>
{
public:
	typedef std::shared_ptr<TCPConnection> pointer;

	static pointer create(asio::io_context& io_context);

	tcp::socket& socket();

	void start();

	static TCPServer* main_server;

	bool is_connected();

private:
	TCPConnection(asio::io_context& io_context);

	void handle_write(const asio::error_code& /*error*/);
	void handle_read_header(const asio::error_code& /*error*/);
	void handle_read_body(const asio::error_code& /*error*/);

	bool send_packet(Packet::Type type, std::vector<char> &data) {
		if (is_connected()) {
			//message_ = make_daytime_string();
			asio::async_write(socket_, asio::buffer(data),
				std::bind(&TCPConnection::handle_write, shared_from_this(),
					std::placeholders::_1 // placeholder for error
					));
			return true;
		}
		else {
			return false;
		}
	}

private:
	tcp::socket socket_;

	Packet::Type in_packet_type;

	UUID uuid;

	// packet about 300-400 bytes (512 safe)
	// 512 bytes is on the larger side for an ordinary tcp tx/rx packet
	// zip ~ 62%

};

