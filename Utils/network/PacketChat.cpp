#include <iostream>
#include "../Packet.h"

PacketChat::PacketChat() {

}

PacketChat::PacketChat(std::string message) : message(message) {
}

// assumes that the identifying ushort has already been read
void PacketChat::read(ByteReader &reader) {
	reader.readString(message);
}

// assumes nothing, writes from scratch
void PacketChat::write(ByteWriter &writer) {

	//printf("called PacketChat::write()\n");

	writer.writeUnsignedShort(Packet::SRC_CLIENT_CHAT);
	writer.writeString(this->message);

	
}

void PacketChat::print() {
	std::cout << "Chat: " << message << "\n";
}