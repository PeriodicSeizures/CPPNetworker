#include "SpriteEngine.h"



namespace SpriteEngine {

	float CAMERA_X = 0, CAMERA_Y = 0;
	float CAMERA_SCALE = 3;
	
	SDL_Texture* sprite_sheet;

	Sprite::Sprite(float x, float y, 
		uint8_t sheet_x, uint8_t sheet_y, 
		uint8_t frame_count)
	: x(x), y(y), 
		vx(0), vy(0),
		sheet_x(sheet_x), sheet_y(sheet_y), 
		sheet_w(16), sheet_h(16),
		frame_count(frame_count),
		frame_time(1000) { }

	Sprite::Sprite(float x, float y, 
		uint8_t sheet_x, uint8_t sheet_y,
		uint8_t sheet_w, uint8_t sheet_h, 
		uint8_t frame_count) 
	: x(x), y(y), 
		vx(0), vy(0),
		sheet_x(sheet_x), sheet_y(sheet_y),
		sheet_w(sheet_w), sheet_h(sheet_h),
		frame_count(frame_count), 
		frame_time(1000) { }

	void Sprite::render() {
		// is correctly time scaled, with decent accuracy
		unsigned int ticks = SDL_GetTicks();
		// the current frame will be decided by the ticks, and frame time
		// the time for each frame is frame_time
		unsigned int frame = (ticks / frame_time) % (frame_count);
		SDL_Rect srcrect = { sheet_x + frame*sheet_w, sheet_y, sheet_w, sheet_h };

		SDL_Rect dstrect = { CAMERA_X+x - sheet_w/2 - ((float)sheet_w * CAMERA_SCALE)/2.0f, 
			CAMERA_Y-y - sheet_h/2 - ((float)sheet_h * CAMERA_SCALE)/2.0f,
			(float)sheet_w * CAMERA_SCALE, (float)sheet_h * CAMERA_SCALE};
		Engine::drawTexture(sprite_sheet, srcrect, dstrect);

	}

	void init() {
		sprite_sheet = Engine::loadTexture("resources/spritesheet.png");
	}

	void uninit() {
		SDL_DestroyTexture(sprite_sheet);
	}



}