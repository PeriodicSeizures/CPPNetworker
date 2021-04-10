#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include <string>
#include "ByteReader.h"
#include "ByteWriter.h"

// CLIENT
class Packet
{
public:
	static constexpr uint16_t SRC_CLIENT_CHAT = 0;
	static constexpr uint16_t SRC_SERVER_CHAT = 1;
	static constexpr uint16_t TCP_FIRE = 2;
	static constexpr uint16_t UDP_TRANSFORM = 2000;
	//num class Type {
	//	// what packets can there be?
	//	CHAT = 0,
	//	TRANSFORM = 1,
	//	FIRE = 2,
	//;

public:

	// for the packet to read data from a bytereader object
	virtual void read(ByteReader &reader) = 0;

	virtual void write(ByteWriter &writer) = 0;

	virtual void print();
};

class PacketChat : public Packet
{
private:
	//char message[64];
	std::string message;

public:
	PacketChat();
	PacketChat(std::string message);

public:

	void read(ByteReader &reader) override;

	void write(ByteWriter &writer) override;

	void print() override;
};



#endif