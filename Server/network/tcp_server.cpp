#include "tcp_server.h"

std::thread TCPServer::run_thread;
asio::io_context TCPServer::_io_context;

TCPServer::TCPServer(unsigned short port) : 
	_acceptor(_io_context, tcp::endpoint(tcp::v4(), port))
{
	do_accept();

	//TCPServer::_io_context.run();
}

TCPServer::~TCPServer() {
	_io_context.stop();
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

void TCPServer::tick() {
	for (auto it = connections.cbegin(); it != connections.cend(); ) {
		//for (auto&& entry : server.connections) {

			/*
			* it is safe to remove every packet, except for the newest one
			* the newest packet might be unfinished
			*/
		if (!it->second->is_connected()) {
			it = connections.erase(it);
			continue;
		}

		const std::shared_ptr<TCPConnection> &conn = it->second;

		while (it->second->in_packets.count() > 1) {
			auto e = it->second->in_packets.pop_front();
			uint16_t len = 0;
			Packet::ErrorCode ec = Packet::S(e.type, len);

			//if (ec == Packet::ErrorCode::OK) {

			switch (e.type) {
			case Packet::Type::CHAT32: {
				// instead of copying the contents of the packet data
				// into the real packet, reinterpret as that type, explicitly
				Packet::Chat32 *chat = static_cast<Packet::Chat32*>((void*)e.data);

				if (chat->target == '\0') {
					// message all clients
					//conn->out_packets.push_back()
				}
				else {
					// message a particular client
				}
				std::cout << "chat32: " << chat->message << "\n";
				break;
			} case Packet::Type::CHAT64: {

			}

			}
			//entry.second->in_packets.p

		}
		++it;
	}
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

			auto conn = std::make_shared<TCPConnection>(std::move(socket), uuid);
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

