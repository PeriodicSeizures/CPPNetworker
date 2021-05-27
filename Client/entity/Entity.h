#pragma once
#include "../engine/Engine.h"
class Entity {
protected:
	float x, y;
	float vx, vy;

	Engine::Sprite sprite;
	uint8_t cur_frame; // the frame index of the current animation
	uint8_t cur_anim; // modify for user
	uint32_t cur_time; // for timings

public:
	Entity(float x, float y, std::string json_filename);

public:
	//virtual void onTick() = 0;
	virtual void onRender();
};

class Player : Entity {
private:
	SDL_Texture* halo;

public:
	Player(float x, float y);
	~Player();

	//void onTick() override;
	void onRender() override;
};