#include "Entity.h"

Player::Player(float x, float y) : Entity(x, y, "resources/ant.json") {	
	halo = Engine::loadTexture("resources/halo.png");
}

Player::~Player() {
	SDL_DestroyTexture(halo);
}

void Player::onRender() { 
	// render my own halo, then render normallyish

	SDL_Rect srcrect = { 0, 0, 64, 64 };
	//SDL_Rect dstrect = { Engine::CAMERA_X + x - 32 - (32.0f * Engine::CAMERA_SCALE) / 2.0f,
	//	Engine::CAMERA_Y - y - 32 - (32.0f * Engine::CAMERA_SCALE) / 2.0f,
	//	64.0f * Engine::CAMERA_SCALE, 64.0f * Engine::CAMERA_SCALE };

	SDL_Rect dstrect = { 
		x - 32*Engine::CAMERA_SCALE,
		-y - 32*Engine::CAMERA_SCALE,
		64.0f * Engine::CAMERA_SCALE, 64.0f * Engine::CAMERA_SCALE };
	
	Engine::drawTexture(halo, srcrect, dstrect);

	Entity::onRender();
}