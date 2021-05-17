#pragma once
#include <asio.hpp>
#include <set>
#include "tcp_connection.h"
#include "AsyncQueue.h"
#include "OwnedPacket.h"

using namespace asio::ip;

class TCPServer
{
public:
	TCPServer(asio::io_context& io_context);

private:
	void start_accept();

	void handle_accept(TCPConnection::pointer new_connection,
		const asio::error_code& e);

	tcp::acceptor acceptor_;
	asio::io_context& io_context_;

public:
	bool send_packet(TCPConnection::pointer client, Packet::Type type, void* data);

	AsyncQueue<OwnedPacket*> incoming_packets;

	std::unordered_map<UUID, 
		TCPConnection::pointer> connections;

	bool verify(TCPConnection::pointer connection);
};