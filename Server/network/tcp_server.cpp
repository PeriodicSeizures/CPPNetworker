#include "tcp_server.h"

TCPServer::TCPServer(unsigned short port) : 
	_acceptor(_io_context, tcp::endpoint(tcp::v4(), port))
{
	start_accept();
	_io_context.run();
}

void TCPServer::start_accept() 
{
	//std::hash<std::string> hasher;
	//UUID uuid = hasher(socket_.remote_endpoint().address().to_string());

	TCPConnection::pointer new_connection =
		TCPConnection::create(_io_context);

	_acceptor.async_accept(new_connection->socket(),
		std::bind(&TCPServer::handle_accept, 
			this, 
			new_connection,
			std::placeholders::_1)
	);
}

void TCPServer::handle_accept(TCPConnection::pointer new_connection,
	const std::error_code& e)
{
	if (!e)
	{
		std::string addr = new_connection->socket().remote_endpoint().address().to_string();
		//if (ip_ban_list.find(addr) == ip_ban_list.end()) {
			// add to list
			//unverified.push_back(new_connection);
			std::hash<std::string> hasher;
			connections[hasher(addr)] = new_connection;
			new_connection->start_reading();
		//}

	}

	this->start_accept(); // loops back to give work
}

//void TCPServer::send_packet(TCPConnection::pointer connection, Packet::Type type, void* data) {
//	connection->send_packet();
//}