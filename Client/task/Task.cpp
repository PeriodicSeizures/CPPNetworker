#include "Task.h"

void Task::focus() {
	prev_task = current_task;
	current_task = this;
}

/*
* static defines
*/

Task* Task::current_task;
Task* Task::prev_task;
TCPConnection::pointer Task::connection;