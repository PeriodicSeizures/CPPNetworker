#include "Player.h"

Player::Player() : SpriteEngine::Sprite(400, -300, 0, 32, 32, 32, 3) {
	Sprite::frame_time = 500; // test #
	halo = Engine::loadTexture("resources/halo.png");
}

void Player::render() { 
	// render my own halo, then render normallyish
	SDL_Rect srcrect = { 0, 0, 64, 64 };
	SDL_Rect dstrect = { SpriteEngine::CAMERA_X + x-32 - ((float)sheet_w * SpriteEngine::CAMERA_SCALE)/2.0f,
		SpriteEngine::CAMERA_Y - y - 32 - ((float)sheet_h * SpriteEngine::CAMERA_SCALE)/2.0f,
		64.0f * SpriteEngine::CAMERA_SCALE, 64.0f * SpriteEngine::CAMERA_SCALE };
	
	Engine::drawTexture(halo, srcrect, dstrect);

	Sprite::render();
}