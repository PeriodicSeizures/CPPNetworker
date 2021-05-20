#ifndef TASK_H
#define TASK_H

#include <SDL.h>

#include "network/tcp_connection.h"
#include "../gui/GUIElement.h"

class Task {

public:
	virtual void on_render() = 0;
	virtual void on_tick() = 0;
	virtual void on_event(SDL_Event &e) = 0;

	/*
	* shared across tasks
	*/
	static Task* current_task;
	static Task* prev_task;
	static asio::io_context _io_context;
	static TCPConnection::pointer connection;

	virtual void focus();
};

class WorldTask : public Task {
public:
	void on_render() override;
	void on_tick() override;
	void on_event(SDL_Event& e) override;

};

/*
* abstract task for main menu and pause menu
* since both have ui elements
* and clickable shit
*/
class GUITask : public Task {
public:

	static GUITextInput* prompted;

	std::vector<GUIElement> elements;

	virtual void on_render() override = 0;
	virtual void on_tick() override = 0;
	virtual void on_event(SDL_Event& e) override = 0;
};

class MainMenuTask : public GUITask {
public:
	void on_render() override;
	void on_tick() override;
	void on_event(SDL_Event& e) override;
};

class PauseMenuTask : public GUITask {
public:
	void on_render() override;
	void on_tick() override;
	void on_event(SDL_Event& e) override;
};

class CommandTask : public Task {
public:
	GUITextInput input;

	void on_render() override;
	void on_tick() override;
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