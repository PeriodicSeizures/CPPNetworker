#include "tcp_server.h"

TCPServer::TCPServer(asio::io_context& io_context) 
	: io_context_(io_context), 
	acceptor_(io_context, tcp::endpoint(tcp::v4(), 13))
{
	start_accept();
}

void TCPServer::start_accept() 
{
	TCPConnection::pointer new_connection =
		TCPConnection::create(io_context_);

	acceptor_.async_accept(new_connection->socket(),
		std::bind(&TCPServer::handle_accept, this, new_connection,
			std::placeholders::_1));
}

void TCPServer::handle_accept(TCPConnection::pointer new_connection,
	const asio::error_code& e)
{
	if (!e)
	{
		// add to list
		connections[(new_connection);
		new_connection->start();
	}

	this->start_accept(); // loops back to give work
}

bool TCPServer::send_packet(TCPConnection::pointer client, Packet::Type type, void* data) {

}