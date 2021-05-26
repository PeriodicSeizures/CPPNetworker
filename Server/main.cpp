/*
	Packet based UDP Server
*/

#include <stdio.h>
#include <iostream>
#include <string>
//#include ""
#include "network/tcp_connection.h"
#include "network/tcp_server.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"

int main()
{
	TCPServer server(13);

	server.start();

	/*
	* poll incoming packets on main thread
	*/
	while (true) {
		for (auto&& entry : server.connections) {

			/*
			* it is safe to remove every packet, except for the newest one
			* the newest packet might be unfinished
			*/
			while (entry.second->in_packets.count() > 1) {
				auto e = entry.second->in_packets.pop_front();
				uint16_t len = 0;
				Packet::ErrorCode ec = Packet::S(e.type, len);
	
				//if (ec == Packet::ErrorCode::OK) {
	
					switch (e.type) {
					case Packet::Type::CHAT32: {
						Packet::Chat32 chat;
						std::memcpy(&chat, e.data, len);
						std::cout << "chat32: " << chat.message << "\n";
					}
	
					}
					//entry.second->in_packets.p
				
			}
		}
	}

	TCPServer::run_thread.join();

	return 0;
}