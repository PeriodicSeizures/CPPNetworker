#ifndef GUIELEMENT_H
#define GUIELEMENT_H

#include <string>
#include <SDL.h>
#include "../engine/Engine.h"

class GUIElement
{
protected:
	int x, y;

public:
	GUIElement(int x, int y);

	virtual void on_render();
	virtual void on_event(SDL_Event& e);
};

/*
* just text
*/
class GUILabel final : public GUIElement {
private:
	int size;
	bool centered;
	std::string text;

	// text color
	static constexpr SDL_Color color = {
		127, 127, 127, 255
	};

public:
	GUILabel(int x, int y,
		int size, bool centered, 
		std::string text);

	void on_render() override;
};

class GUIButton final : public GUIElement {
private:
	int size;
	GUILabel label;
	int w, h;
	void (*callback)();

	// backer color
	static constexpr SDL_Color color = {
		255, 255, 255, 255
	};

public:
	GUIButton(int x, int y,
		int size,
		std::string text,
		int w, int h,
		void (*callback)());

	void on_render() override;
	void on_event(SDL_Event& e) override;
};

class GUITextInput final : public GUIElement {
public:
	int size;
	int max_chars;
	std::string text;

	// text color
	static constexpr SDL_Color color = {
		127, 127, 127, 255
	};

public:
	GUITextInput(int x, int y,
		int size, int max_chars, 
		std::string text);

	void on_render() override;
	void on_event(SDL_Event& e) override;
};

#endif