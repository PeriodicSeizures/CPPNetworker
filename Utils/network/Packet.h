#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>
#include <string>
#include <assert.h>
#include "ByteReader.h"
#include "ByteWriter.h"
#include "../Common.h"

/* * * * * * * * * * * * * * * * * * * * * * * *

					PACKETS

 * * * * * * * * * * * * * * * * * * * * * * * */



struct Packet {

	struct ConnectionRefused {
		char message[16];
		//void onReceive(uint16_t user) override;
	};

	struct ConnectionVersion {
		char message[16];
		//void onReceive(uint16_t user) override;
	};

	struct ConnectionLogin {
		char username[16];
		char password[16];
		//void onReceive(uint16_t user) override;
	};

	struct Chat32 {
		char message[32];
		char target[16]; // optional 
		//void onReceive(uint16_t user) override;
	};

	struct Chat64 {
		char message[64];
		char target[16]; // optional 
		//void onReceive(uint16_t user) override;
	};

	struct Chat128 {
		char message[128];
		char target[16]; // optional 
		//void onReceive(uint16_t user) override;
	};

	struct Chat256 {
		char message[256];
		char target[16]; // optional 
		//void onReceive(uint16_t user) override;
	};

	// for clients that are not exploitive (trust)
	struct TrustedMotion { // client calculates motion (cheaper for server)
		float x, y;
		float vx, vy;
		Direction dir;
		//void onReceive(uint16_t user) override;
	};

	// for clients that might be exploitive (ignore)
	struct UnTrustedMotion { // server calculates motion (expensive for server)
		Input input;
		//void onReceive(uint16_t user) override;
	};

	enum class Type : uint16_t {
		SRC_SERVER_CONNECTION_REFUSED,
		SRC_SERVER_CONNECTION_VERSION,	// version of server
		SRC_CLIENT_CONNECTION_LOGIN,	// identity of client
		CHAT32,		// sent by client to server
		CHAT64,
		CHAT128,
		CHAT256,
		SRC_CLIENT_TRUSTED_MOTION,
		SRC_CLIENT_UNTRUSTED_MOTION,
		count // kind of hacky
	};

	static constexpr unsigned int SIZE = sizeof(Type);

	static uint16_t S(Type type);

	//template<typename T>
	//static void serialize(T& in, char* out) {
	//	static_assert(std::is_pod<T>::value, "in is not a POD type");
	//
	//	std::memcpy(out, (void*)&in, sizeof(T));
	//}

	/*
	* 
	* Members
	* 	
	*/

	Type type;
	char* data;

private:
	static constexpr size_t sizes[] = {
		sizeof(ConnectionRefused),
		sizeof(ConnectionVersion),
		sizeof(ConnectionLogin),
		sizeof(Chat32),
		sizeof(Chat64),
		sizeof(Chat128),
		sizeof(Chat256),
		sizeof(TrustedMotion),
		sizeof(UnTrustedMotion),
	};
};

struct OwnedPacket {
	UUID uuid;
	Packet packet;
};

#endif