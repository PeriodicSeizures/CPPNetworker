#pragma once
#include <asio.hpp>
#include <set>
#include "network/tcp_connection.h"

using namespace asio::ip;

class TCPServer
{
private:
	tcp::acceptor _acceptor;
	asio::io_context _io_context;
	std::unordered_map<UUID, TCPConnection::pointer> connections;

public:
	TCPServer();
	void send_packet(TCPConnection::pointer client, Packet::Type type, void* data);
	//void process_packet(TCPConnection::pointer client, Packet packet);

private:
	void start_accept();
	void handle_accept(TCPConnection::pointer new_connection,
		const asio::error_code& e);

};