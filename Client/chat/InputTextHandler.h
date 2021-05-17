#pragma once
#include <string>
#include <SDL.h>
class InputTextHandler
{
private:
	bool enabled = false;
	//int sel = 0, len = 0;
	std::string message;

public:
	bool processInput(const SDL_Event& e);

	void render();

	bool isEnabled();

	void enable();

private:
	void disable();

	void sendToServer();
};

