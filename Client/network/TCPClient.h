#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include "AsyncQueue.h"
#include "network/Packet.h"

#ifdef _WIN32
#define _WIN32_WINNT 0xA00
#endif
#define ASIO_STANDALONE
#include <asio.hpp>

using namespace asio::ip;

class TCPClient
{
private:
	tcp::socket _socket;
	asio::io_context _io_context;
	AsyncQueue<Packet> in_packet_queue;

	Packet::Type in_packet_type; // received

	AsyncQueue<Packet> out_packet_queue; // to send

public:
	TCPClient(std::string host, std::string port);

	void send_packet(Packet &packet);

private:
	void do_connect(const tcp::resolver::results_type& endpoints);

	void handle_read_header(const asio::error_code& e);
	void handle_read_body(const asio::error_code& e);
	//void do_write(const asio::error_code& e);
	void handle_send_header(const asio::error_code& e);
	void handle_send_body(const asio::error_code& e);
};

#endif