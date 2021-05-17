#pragma once
#include <stdint.h>
#include <SDL.h>
#include "Engine.h"

namespace SpriteEngine {

	extern float CAMERA_X, CAMERA_Y;
	extern float CAMERA_SCALE;

	extern SDL_Texture* sprite_sheet;

	class Sprite {
	protected:
		float x, y;
		float vx, vy;
		unsigned int sheet_x, sheet_y;
		unsigned int sheet_w, sheet_h;
		unsigned int frame_count;
		unsigned int frame_time; // fair const
	public:
		Sprite(float x, float y, uint8_t sheet_x, uint8_t sheet_y, uint8_t frame_count = 1);
		Sprite(float x, float y, uint8_t sheet_x, uint8_t sheet_y, uint8_t sheet_w, uint8_t sheet_h, uint8_t frame_count = 1);

		virtual void render();
	};

	void init();
	void uninit();

}