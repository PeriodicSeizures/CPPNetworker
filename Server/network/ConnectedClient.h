#pragma once
#include <winsock2.h>
struct ConnectedUDPClient {
	//char addr[14]; // max of 14
	sockaddr_in addr;
	int addr_len;
	//unsigned long ul_addr; // a long representation of the ip
	// client.sin_addr.s_addr

	const char* getIP();

};

struct ConnectedTCPClient {
	SOCKET sock;

};