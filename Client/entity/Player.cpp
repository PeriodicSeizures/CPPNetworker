#include "Entity.h"

Player::Player(float x, float y) : Entity(x, y, "resources/ant.json") {	
	halo = Engine::loadTexture("resources/halo.png");
}

Player::~Player() {
	SDL_DestroyTexture(halo);
}

void Player::on_render() { 
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

	Entity::on_render();
}

static constexpr uint8_t UP_MASK =		0x1;
static constexpr uint8_t RIGHT_MASK =	0x01;
static constexpr uint8_t DOWN_MASK =	0x001;
static constexpr uint8_t LEFT_MASK =	0x0001;

static constexpr double ANGLES[] = {
	0, 90, 45, 0, -45, -90, -135, -180, -225
};

void Player::on_update(float delta) {
	SDL_PumpEvents();
	const Uint8* keystate = SDL_GetKeyboardState(NULL);

	do_animate = false;
	
	//bool up = false, right = false, down = false, left = false;

	uint8_t mask = 0;

	if (keystate[SDL_SCANCODE_W]) {
		do_animate = true;
		this->ay = 700;
		mask |= UP_MASK;
	}
	else if (keystate[SDL_SCANCODE_S]) {
		do_animate = true;
		this->ay = -700;
		mask |= DOWN_MASK;
	}
	else {
		this->ay = 0;
	}

	if (keystate[SDL_SCANCODE_D]) {
		do_animate = true;
		this->ax = 700;
		mask |= RIGHT_MASK;
	}
	else if (keystate[SDL_SCANCODE_A]) {
		do_animate = true;
		this->ax = -700;
		mask |= LEFT_MASK;
	}
	else {
		this->ax = 0;
	}

	double new_angle = 360 - ANGLES[mask];

	double ac = (angle - new_angle) * delta;
	angle += ac;

	angle = fmod(angle, 360);
	if (angle < 0)
		angle += 360;

	Entity::on_update(delta);
}

//void Player::onEvent(SDL_Event& e) {
//
//	switch (e.type) {
//	case SDL_KEYDOWN: {
//	
//		switch (e.key.keysym.sym) {
//		case SDLK_w: // forward
//			this->ay = 400;
//			break;
//		case SDLK_s:
//			this->ay = -400;
//			break;
//		case SDLK_a:
//			this->ax = -400;
//			break;
//		case SDLK_d:
//			this->ax = 400;
//			break;
//		}
//		break;
//	
//	}
//	case SDL_KEYUP: {
//	
//		switch (e.key.keysym.sym) {
//		case SDLK_w: // forward
//		case SDLK_s:
//			this->ay = 0;
//			break;
//		case SDLK_a:
//		case SDLK_d:
//			this->ax = 0;
//			break;
//		}
//		break;
//	
//	}
//	
//	}
//}