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

	//void send(const Packet& packet);

	tcp::socket socket_;

	//const unsigned int id;

	// zip ~ 62%

	Packet::Type in_packet_type;

	static constexpr uint32_t HEADER_SIZE = 2; // fixed

	UUID uuid;

	// packet about 300-400 bytes (512 safe)
	// 512 bytes is on the larger side for an ordinary tcp tx/rx packet

};

