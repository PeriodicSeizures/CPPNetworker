#pragma once

#include <vector>
#include "Packet.h"

struct OwnedPacket
{
	Packet::Type type;
	std::vector<char> data;
	uint16_t owner;
};

