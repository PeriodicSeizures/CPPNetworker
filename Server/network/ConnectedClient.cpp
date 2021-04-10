#include <ws2tcpip.h>
#include "ConnectedClient.h"

static char ipbuf[32];

const char* ConnectedUDPClient::getIP() {
	
	inet_ntop(AF_INET,
		&addr.sin_addr,
		ipbuf, sizeof(ipbuf));

	//ntohs(addr.sin_port);
	return ipbuf;
}