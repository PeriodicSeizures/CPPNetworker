#include "Entity.h"
#include "network/Packet.h"
#include "../task/Task.h"

Player::Player(float x, float y) : Entity(x, y, "resources/ant.json"), 
vignette_sprite("resources/vig.png") {	
	//halo = Engine::loadTexture("resources/halo.png");
	//vignette_sprite = Engine::loadTexture("resources/vig.png");
}

Player::~Player() {
	//SDL_DestroyTexture(halo);
	//SDL_DestroyTexture(vignette);
}

void Player::on_render() { 
	// render my own halo, then render normallyish

	//SDL_Rect srcrect = { 0, 0, 64, 64 };
	//SDL_Rect dstrect = { 
	//	x - 32*Engine::CAMERA_SCALE,
	//	-y - 32*Engine::CAMERA_SCALE,
	//	64.0f * Engine::CAMERA_SCALE, 64.0f * Engine::CAMERA_SCALE };
	
	//Engine::drawTexture(halo, srcrect, dstrect);
	// 
	//SDL_Color black = {0, 0, 0, 255};
	//Engine::drawTexture(black, dstrect.x, dstrect.y, dstrect.w, dstrect.h);

	vignette_sprite.draw(x, y, 0);

	Entity::on_render();
}

static constexpr uint8_t UP_MASK =		0b0001;
static constexpr uint8_t RIGHT_MASK =	0b0010;
static constexpr uint8_t DOWN_MASK =	0b0100;
static constexpr uint8_t LEFT_MASK =	0b1000;

static constexpr double ANGLES[] = {
	-999, 90, 0, 45, -90, -999, -45, -999, -180, 135, -999, -999, -135
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

	if (do_animate && Task::connection) {
		// send packets
		Packet::TrustedMotion packet = { x, y, vx, vy, ax, ay };
		
		Task::connection->dispatch(std::move(packet));
	}

	//angle = 360 - ANGLES[mask] + 90;

	auto new_angle = ANGLES[mask];

	if (new_angle != -999) {
		// then try turning
		new_angle = 360 - new_angle + 90;
		angle = new_angle;
		//auto diff = new_angle - angle;
		//auto change = diff * delta;
		//if (fabs(change) < .01)
		//	change = .01;
		////if (change > .005 && change < .5)
		//	//change = .05;
		////if (fabs(diff) < 180)
		//	//change *= -1;
		//angle += change;
	}

	//angle = fmod(angle, 360);
	//if (angle < 0)
	//	angle += 360;
		

	//double new_angle = 360 - ANGLES[mask];
	//
	//double ac = (angle - new_angle) * delta;
	//angle += ac;
	//
	//angle = fmod(angle, 360);
	//if (angle < 0)
	//	angle += 360;

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