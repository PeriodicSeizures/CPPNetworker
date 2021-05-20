#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <asio.hpp>
#include "network/tcp_connection.h"

using namespace asio::ip;

class TCPServer
{
private:
	// Order of these is apparently important 
	std::unordered_map<UUID, TCPConnection::pointer> connections;

	asio::io_context _io_context;
	tcp::acceptor _acceptor;	

public:
	TCPServer(unsigned short port);
	void send_packet(TCPConnection::pointer client, Packet::Type type, void* data);
	//void process_packet(TCPConnection::pointer client, Packet packet);

private:
	void start_accept();
	void handle_accept(TCPConnection::pointer new_connection,
		const std::error_code& e);

};

#endif
