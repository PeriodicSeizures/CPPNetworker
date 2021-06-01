#ifndef TASK_H
#define TASK_H

#include <thread>
#include <SDL.h>
#include "network/tcp_connection.h"
#include "../gui/GUIElement.h"
#include "../entity/Entity.h"

class Task {

public:
	virtual void on_render() = 0;
	virtual void on_update(float delta) = 0;
	virtual void on_event(SDL_Event &e) = 0;

	/*
	* shared variables for tasks
	*/
	static Task* current_task;
	static Task* prev_task;
	static asio::io_context _io_context;
	static std::shared_ptr<TCPConnection> connection;

	static std::thread run_thread;
	static std::condition_variable cv_run;
	static std::mutex mux_run;
	static bool running; // for asio
	static bool do_stops;

	/*
	* in game variables
	*/

	static Player *player;
	static Player *other; // = new Player(400, -300);
	static Engine::Sprite* brick_sprite;

	/*
	* Settings
	*/
	static bool DEBUG;
	static bool GAME_ALIVE;

	static void init();
	static void uninit();

	virtual void focus();
};

class WorldTask : public Task {
public:
	WorldTask();
	void on_render() override;
	void on_update(float delta) override;
	void on_event(SDL_Event& e) override;

	void do_listen();

	//Player* other;

	//Engine::Sprite *brick_sprite;
};

/*
* abstract task for main menu and pause menu
* since both have ui elements
* and clickable shit
*/
class GUITask : public Task {
public:
	GUITask();
	static GUITextInput* prompted;
	static bool any_focused;

	std::vector<GUIElement*> elements;

	virtual void on_render() override;
	void on_update(float delta) override;
	virtual void on_event(SDL_Event& e) override;
};

class MainMenuTask : public GUITask {
public:
	MainMenuTask();

	void on_render() override;
	void on_update(float delta) override;
	void on_event(SDL_Event& e) override;
};

class PauseMenuTask : public GUITask {
public:
	PauseMenuTask();

	void on_render() override;
	void on_update(float delta) override;
	void on_event(SDL_Event& e) override;
};

class CommandTask : public GUITask {
public:
	CommandTask();

	void on_render() override;
	void on_update(float delta) override;
	void on_event(SDL_Event& e) override;

};

/*
* PRIMARY TASK DECLARATIONS
*/

extern WorldTask WORLD_TASK;
extern MainMenuTask MAIN_MENU_TASK;
extern PauseMenuTask PAUSE_MENU_TASK;
extern CommandTask COMMAND_TASK;

#endif