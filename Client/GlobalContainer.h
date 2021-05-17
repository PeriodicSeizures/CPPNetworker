#pragma once
#include "network/TCPClient.h"
#include "chat/InputTextHandler.h"
class GlobalContainer
{
public:
	static TCPClient tcp_client;
	static InputTextHandler input_text_handler;
};

