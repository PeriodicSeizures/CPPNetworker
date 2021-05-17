#pragma once
#include <asio.hpp>
#include <set>
#include "tcp_connection.h"
#include "AsyncQueue.h"
#include "network/OwnedPacket.h"

using namespace asio::ip;

class TCPServer
{
public:
	TCPServer(asio::io_context& io_context);

private:
	void start_accept();

	void handle_accept(TCPConnection::pointer new_connection,
		const asio::error_code& e);

	tcp::acceptor acceptor_; // (io_context, tcp::endpoint(tcp::v4(), 13));
	asio::io_context& io_context_;

	//std::set<std::string> ip_ban_list;
	//std::set<std::string> name_ban_list;

	//std::unordered_map<std::string, std::string> user_map;

public:
	bool send_packet(TCPConnection::pointer client, Packet::Type type, void* data);

	AsyncQueue<OwnedPacket*> incoming_packets;

	//AsyncQueue<TCPConnection::pointer> unverified;

	std::unordered_map<UUID, 
		TCPConnection::pointer> connections;

	bool verify(TCPConnection::pointer connection);
};