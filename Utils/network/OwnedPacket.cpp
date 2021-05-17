#include "OwnedPacket.h"

OwnedPacket::OwnedPacket(Packet::Type type, std::vector<char> data, uint16_t owner)
	: type(type), data(data)owner_id(owner_id) {

}

OwnedPacket::~OwnedPacket() {
	delete packet;
}

const Packet* OwnedPacket::getPacket() {
	return packet;
}

uint16_t OwnedPacket::getOwnerId() {
	return owner_id;
}