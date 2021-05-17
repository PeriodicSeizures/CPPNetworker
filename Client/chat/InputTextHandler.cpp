#include "InputTextHandler.h"
#include "../engine/Engine.h"
#include "../GlobalContainer.h"



bool InputTextHandler::processInput(const SDL_Event &e) {

	if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym) {
		case SDLK_BACKSPACE:

			if (!enabled)
				return false;

			if (!message.empty())
				message.pop_back();
			break;
		case SDLK_RETURN:

			if (!enabled) {
				enable();
				return true;
			}

			if (SDL_GetModState() & KMOD_SHIFT) {
				message += '\n';
				break;
			}

			// then send the message only if it has content
			if (!message.empty()) {
				PacketChat packet(message);
				GlobalContainer::tcp_client.sendPacket(&packet);
			}
			disable();
			break;
		case SDLK_ESCAPE:

			if (!enabled)
				return false;

			this->disable();
			break;
		case SDLK_c:

			if (!enabled)
				return false;

			if (SDL_GetModState() & KMOD_CTRL)
				SDL_SetClipboardText(message.c_str());
			break;
		case SDLK_v:

			if (!enabled)
				return false;

			if (SDL_GetModState() & KMOD_CTRL)
				message += SDL_GetClipboardText();
			break;
		}
	}
	else if (e.type == SDL_TEXTINPUT && enabled)
	{


		//Not copy or pasting
		if (!(SDL_GetModState() & KMOD_CTRL && (e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
		{
			//Append character
			message += e.text.text;
		}
	}

	//SDL_StopTextInput();

	//switch (e.key.keysym.sym) {
	//case SDLK_a:
	//
	//}
	return true;
}

bool InputTextHandler::isEnabled() {
	return enabled;
}

void InputTextHandler::enable() {

	printf("taking chat input\n");

	if (!enabled)
		SDL_StartTextInput();

	enabled = true;
}

void InputTextHandler::disable() {

	printf("stopped chat input\n");

	if (enabled)
		SDL_StopTextInput();

	message.clear();

	enabled = false;
}

void InputTextHandler::render() {
	Engine::drawFormattedString(message, 4, 4, 0xFF, 2, false);
}

void InputTextHandler::sendToServer() {
	PacketChat packet;
	GlobalContainer::tcp_client.sendPacket(&packet);
}