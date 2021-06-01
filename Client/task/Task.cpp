#include "Task.h"

/*
* static defines
*/

Task* Task::current_task = nullptr;
Task* Task::prev_task = nullptr;
asio::io_context Task::_io_context;
std::shared_ptr<TCPConnection> Task::connection;

std::thread Task::run_thread;
std::condition_variable Task::cv_run;
std::mutex Task::mux_run;
bool Task::running = true;
bool Task::do_stops = false;

Player* Task::player; // (400, -300);
Player* Task::other; // (400, -300);
Engine::Sprite* Task::brick_sprite;

bool Task::DEBUG;
bool Task::GAME_ALIVE = true;

void Task::focus() {
	prev_task = current_task;
	current_task = this;


	//_io_context.ru
}

void Task::init() {
	connection = std::make_shared<TCPConnection>(Task::_io_context);
	
	//Task::_io_context.run_one(); // dummy run

	Task::run_thread = std::thread([]() {
		while (Task::running) {
			std::unique_lock<std::mutex> u1(Task::mux_run);
			Task::cv_run.wait(u1);

			Task::_io_context.run();
			Task::_io_context.restart(); // this will only run if the io_context is stopped
		}
	});

	//player = new Player(400, -300);
	player = new Player(400, -300);
	other = new Player(400, -300);
	brick_sprite = new Engine::Sprite("resources/brick.json");
}

void Task::uninit() {
	// connection is a smart ptr, no need to free it
	Task::running = false;
	Task::cv_run.notify_one();
	if (Task::do_stops) {
		Task::_io_context.stop();
	}
	Task::run_thread.join();
}