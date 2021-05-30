#include "Entity.h"

Entity::Entity(float x, float y, std::string json_filename) 
	: x(x), y(y), vx(0), vy(0), ax(0), ay(0), sprite(json_filename) { }

void Entity::on_render() {
	unsigned int ticks = SDL_GetTicks();

	// current ANIMFRAME will be determined by:
	auto t = (ticks - cur_time); // gives a number less than the current dur

	Engine::Animation& anim = sprite.animations[cur_anim];

	if (t >= anim.durations[cur_frame] && do_animate) {
		(++cur_frame) %= anim.durations.size() - 1;
		cur_time = ticks;
	}

	//if (current_frame >= durations.size())
	//    current_frame = 0;


	sprite.draw(this->x, this->y, angle, cur_frame, cur_anim);
}

void Entity::on_update(float delta) {

	vx += ax * delta; // increment the velocity in tick
	vy += ay * delta;

	// terminal velocity
	if (vx > terminal_v)
		vx = terminal_v;
	else if (vx < -terminal_v)
		vx = -terminal_v;

	if (vy > terminal_v)
		vy = terminal_v;
	else if (vy < -terminal_v)
		vy = -terminal_v;

	// friction falloff
	// if the player is being moved by player
	if (abs(ax) < std::numeric_limits<float>::epsilon()) {
		if (vx < 0) vx += fric;
		else vx += -fric;

		if (abs(vx) <= fric) // fric elipson
			vx = 0;
	}

	if (abs(ay) < std::numeric_limits<float>::epsilon()) {
		if (vy < 0) vy += fric;
		else vy += -fric;

		if (abs(vy) <= fric) // fric elipson
			vy = 0;
	}



	x += vx * delta; // increment the position in tick
	y += vy * delta;

}