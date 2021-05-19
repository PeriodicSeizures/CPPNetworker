#include "GlobalContainer.h"

namespace crzi {
	NetworkingGame *NetworkingGame::instance;
	//asio::io_context _io_context;

	NetworkingGame::NetworkingGame()
		/*: connection(TCPConnection::create(_io_context)) */{

	}



	void NetworkingGame::init() {
		NetworkingGame::instance = new NetworkingGame();
	}

}