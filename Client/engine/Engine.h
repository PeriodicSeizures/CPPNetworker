#pragma once
#include <SDL.h>
#include <string>
namespace Engine {

	SDL_Texture* loadTexture(std::string path);

	void init();

	void uninit();

	void doRender();

	void drawTexture(SDL_Texture *texture, SDL_Rect& src_rect, SDL_Rect& rect);

	void drawFormattedString(const std::string& str, 
		int x, int y, 
		uint8_t alpha, int size, bool centered);

	void drawString(const std::string& str, 
		int x, int y, 
		const SDL_Color& color, int size, bool centered);

	void fill(const SDL_Color& color);

	void fillRect(const SDL_Color& color, const SDL_Rect& rect);
}