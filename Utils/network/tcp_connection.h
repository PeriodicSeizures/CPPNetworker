#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <asio.hpp>
#include <memory>
#include <deque>
#include <unordered_map>
#include "Packet.h"
#include "../AsyncQueue.h"

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE 1

using namespace asio::ip;

class TCPConnection : public std::enable_shared_from_this<TCPConnection>
{
private:
	tcp::socket _socket;
	Packet::Type in_packet_type;
	AsyncQueue<Packet> in_packets;
	AsyncQueue<Packet> out_packets;

public:
	typedef std::shared_ptr<TCPConnection> pointer;

	//static void init_io_context(asio::io_context &_io_context);
	static pointer create(asio::io_context& _io_context);

	tcp::socket& socket();
	
	bool is_connected();
	void send_packet(Packet &packet);

	/*
	* Used by server 
	* Client safe, but there's conect_to_server...
	*/
	void start_reading();

	/*
	* To connect to server (used by client)
	* Might cause issues if used on a server architecture
	*/
	void connect_to_server(asio::io_context& _io_context, 
		std::string host, 
		std::string port);

private:
	TCPConnection(asio::io_context& _io_context);

	void read_header();
	void read_body();

	void write_header();
	void write_body();

	// packet about 300-400 bytes (512 safe)
	// 512 bytes is on the larger side for an ordinary tcp tx/rx packet
	// zip ~ 62%

};

#endif
