#ifndef TASK_H
#define TASK_H

#include "network/tcp_connection.h"
#include "../chat/InputTextHandler.h"

class Task {

public:
	virtual void on_render() = 0;
	virtual void on_tick() = 0;
	virtual void on_event() = 0;

	/*
	* shared
	*/
	static Task *current_task;
	static TCPConnection::pointer connection;
	static InputTextHandler input_text_handler;

	virtual void focus();
};

//class SettingTask {
//
//};
//
//class MenuTask {
//
//};

#endif