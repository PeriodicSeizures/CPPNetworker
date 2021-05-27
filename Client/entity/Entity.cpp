#include "Entity.h"

Entity::Entity(float x, float y, std::string json_filename) 
	: x(x), y(y), vx(0), vy(0), sprite(json_filename) { }

void Entity::onRender() {
	unsigned int ticks = SDL_GetTicks();

	// current ANIMFRAME will be determined by:
	auto t = (ticks - cur_time); // gives a number less than the current dur

	Engine::Animation& anim = sprite.animations[cur_anim];

	if (t >= anim.durations[cur_frame]) {
		(++cur_frame) %= anim.durations.size() - 1;
		cur_time = ticks;
	}

	//if (current_frame >= durations.size())
	//    current_frame = 0;


	sprite.draw(this->x, this->y, cur_frame, cur_anim);
}