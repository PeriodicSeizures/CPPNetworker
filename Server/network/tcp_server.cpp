#include "tcp_server.h"

std::thread TCPServer::run_thread;
asio::io_context TCPServer::_io_context;

TCPServer::TCPServer(unsigned short port) : 
	_acceptor(_io_context, tcp::endpoint(tcp::v4(), port))
{
	do_accept();

	//TCPServer::_io_context.run();
}

void TCPServer::start() {
	TCPServer::run_thread = std::thread(
		[]() 
		{
			_io_context.run();
			//TCPServer::_io_context.poll();
			//std::cout << "exited run\n";
			
			std::cout << "exited run\n";
		}
	);
}

void TCPServer::do_accept() 
{
	//std::hash<std::string> hasher;
	//UUID uuid = hasher(socket_.remote_endpoint().address().to_string());

	_acceptor.async_accept(
		[this](const asio::error_code& ec, tcp::socket socket)
	{
		if (!ec)
		{
			std::cout << "connect\n";

			std::hash<std::string> hasher;
			UUID uuid = hasher(socket.remote_endpoint().address().to_string());

			auto conn = std::make_shared<TCPConnection>(std::move(socket));

			connections.insert({ uuid, conn });

			conn->start();

			//asio::steady_timer timer(_io_context);
			//timer.expires_after(std::chrono::seconds(5));
			//timer.async_wait([conn](const asio::error_code&) {
			//	Packet::Chat32 chat32 = {"Hello, World!"};
			//	conn->send_packet(Packet::serialize(chat32, chat32.type));
			//	});

			//std::make_shared<TCPConnection>(_io_context, std::move(socket))->start();
		}
		
		do_accept(); // loops back to continue accept
	});


	/*
	* jsut print all incoming messages 
	*/
	//while (true) {
	//	for (auto&& entry : connections) {
	//		//entry.second->
	//	}
	//}

}