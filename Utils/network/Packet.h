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

	enum class Type : uint16_t {
		DUMMY,
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

	/*
	* 
	* Packet declarations
	* 
	*/

	struct ConnectionRefused {
		static constexpr Packet::Type type = Packet::Type::SRC_SERVER_CONNECTION_REFUSED;
		char message[16];
	};

	struct ConnectionVersion {
		static constexpr Packet::Type type = Packet::Type::SRC_SERVER_CONNECTION_VERSION;
		char message[16];
	};

	struct ConnectionLogin {
		static constexpr Packet::Type type = Packet::Type::SRC_CLIENT_CONNECTION_LOGIN;
		char username[16];
		char password[16];
	};

	struct Chat32 {
		static constexpr Packet::Type type = Packet::Type::CHAT32;
		char message[32];
		char target[16] = ""; // optional 
	};

	struct Chat64 {
		static constexpr Packet::Type type = Packet::Type::CHAT64;
		char message[64];
		char target[16] = ""; // optional 
	};

	struct Chat128 {
		static constexpr Packet::Type type = Packet::Type::CHAT128;
		char message[128];
		char target[16] = ""; // optional 
	};

	struct Chat256 {
		static constexpr Packet::Type type = Packet::Type::CHAT256;
		char message[256];
		char target[16] = ""; // optional 
	};

	// trust the client motion
	struct TrustedMotion { //  (cheaper for server)
		static constexpr Packet::Type type = Packet::Type::SRC_CLIENT_TRUSTED_MOTION;
		float x, y;
		float vx, vy;
		float ax, ay;
		//Direction dir;
		UUID source;
	};

	// fuck the clients motion
	struct UnTrustedMotion { // (expensive for server)
		static constexpr Packet::Type type = Packet::Type::SRC_CLIENT_UNTRUSTED_MOTION;
		Input input;
	};

	/*
	* 
	* Utility methods and types
	* 
	*/

	enum class ErrorCode : uint16_t {
		OK = 0,
		INVALID_HEADER
	};

	// Size of header
	static constexpr unsigned int SIZE = sizeof(Type);

	/*
	* Return the sizeof(...) of a packet 
	* that is correlated to @type
	*/
	static ErrorCode S(Type type, uint16_t &ret);

	//template<typename T>
	//static Packet serialize(T& in, Packet::Type type) {
	//	if (sizes[(uint16_t)type] == 0) {
	//		return { type };
	//	}
	//	else {
	//		Packet packet = { type, new char[sizes[(uint16_t)type]] };
	//		std::memcpy(packet.data, (void*)&in, sizeof(T));
	//		return packet;
	//	}
	//}

	template<typename T>
	static T* deserialize(Packet& in) {
		return static_cast<T*>((void*)in.data);
	}

	/*
	* 
	* Member variables
	* 	
	*/

	Type type;
	char* data;

//private:
	static constexpr size_t sizes[] = {
		0, // dummy packet
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