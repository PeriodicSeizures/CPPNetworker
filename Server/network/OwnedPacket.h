#ifndef OWNED_PACKET_H
#define OWNED_PACKET_H

#include <vector>
#include <asio.hpp>
#include "network/Packet.h"

using namespace asio::ip;

struct OwnedPacket
{
	UUID owner;
	Packet packet;
	//tcp::socket &socket_;
};

#endif