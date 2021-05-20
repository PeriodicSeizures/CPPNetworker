#include "GUIElement.h"

GUITextInput::GUITextInput(int x, int y,
	int size, int max_chars,
	std::string text)
	: GUIElement(x, y), size(size), 
		max_chars(max_chars), text(text) {}

void GUITextInput::on_render() {
	Engine::drawString(text, x, y, color, size, false);
}

void GUITextInput::on_event(SDL_Event &e) {

	if (e.type == SDL_KEYDOWN) {
		switch (e.key.keysym.sym) {
		case SDLK_BACKSPACE: {
			if (!text.empty())
				text.pop_back();
			break;
		} case SDLK_RETURN: {
			if (SDL_GetModState() & KMOD_SHIFT) { // NEWLINE
				text += '\n';
				break;
			}
			// disable();
			break;
		} case SDLK_ESCAPE: {
			// this->disable();
			break;
		} case SDLK_c: {
			if (SDL_GetModState() & KMOD_CTRL)
				SDL_SetClipboardText(text.c_str());
			break;
		} case SDLK_v: {
			if (SDL_GetModState() & KMOD_CTRL)
				text += SDL_GetClipboardText();
			break;
		}
		}
	}
	else if (e.type == SDL_TEXTINPUT && 
		text.size() < max_chars)
	{
		//Not copy or pasting
		if (!(SDL_GetModState() & KMOD_CTRL &&
			(e.text.text[0] == 'c' || e.text.text[0] == 'C' || e.text.text[0] == 'v' || e.text.text[0] == 'V')))
		{
			//Append character
			text += e.text.text;
		}
	}
}