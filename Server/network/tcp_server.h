#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <asio.hpp>
#include "network/tcp_connection.h"

using namespace asio::ip;

class TCPServer
{
public:
	// Order of these is apparently important 
	std::unordered_map<UUID, 
		std::shared_ptr<TCPConnection>> connections;

	static std::thread run_thread;

	static asio::io_context _io_context;
	tcp::acceptor _acceptor;

public:
	TCPServer(unsigned short port);
	//void send_packet(std::shared_ptr<TCPConnection> client, Packet::Type type, void* data);
	//void process_packet(TCPConnection::pointer client, Packet packet);

	void start();

	void tick();

private:
	void do_accept();

};

#endif
