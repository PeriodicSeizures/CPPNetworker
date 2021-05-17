#ifndef OWNED_PACKET_H
#define OWNED_PACKET_H

#include <vector>
#include <asio.hpp>
#include "network/Packet.h"

using namespace asio::ip;

struct OwnedPacket
{
	Packet::Type type;
	std::vector<char> data;
	UUID owner;
	//tcp::socket &socket_;
};

#endif