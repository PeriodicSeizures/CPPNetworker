#ifndef TCP_CONNECTION_H
#define TCP_CONNECTION_H

#include <asio.hpp>
#include <memory>
#include <deque>
#include <unordered_map>
#include "Packet.h"
#include "../AsyncQueue.h"

class TCPServer;

#ifdef _WIN32
#define _WIN32_WINNT 0x0A00
#endif
#define ASIO_STANDALONE

using namespace asio::ip;

class TCPConnection : public std::enable_shared_from_this<TCPConnection>
{
private:
	tcp::socket _socket;
	Packet::Type in_packet_type;
	//UUID uuid;
	AsyncQueue<Packet> in_packets;
	AsyncQueue<Packet> out_packets;

	static asio::io_context* _io_context;

public:
	typedef std::shared_ptr<TCPConnection> pointer;

	static void init_io_context(asio::io_context &_io_context);
	static pointer create();

	tcp::socket& socket();
	void start();
	bool is_connected();
	void send_packet(Packet &packet);

	/*
	* To connect to server (used by client)
	*/
	void connect_to_server(std::string host, std::string port);

	/*
	* To 
	*/

private:
	TCPConnection();

	void handle_read_header(const asio::error_code& e);
	void handle_read_body(const asio::error_code& e);

	void handle_write_header(const asio::error_code& e);
	void handle_write_body(const asio::error_code& e);



	// packet about 300-400 bytes (512 safe)
	// 512 bytes is on the larger side for an ordinary tcp tx/rx packet
	// zip ~ 62%

};

#endif