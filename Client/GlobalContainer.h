#ifndef GAME_H
#define GAME_H

#include "network/tcp_connection.h"
#include "chat/InputTextHandler.h"

namespace crzi {
	class NetworkingGame {
	public:
		TCPConnection::pointer connection;
		InputTextHandler input_text_handler;

		// 

		static NetworkingGame* instance;

	private:
		NetworkingGame();

	public:
		static void init();
	};
}

#endif