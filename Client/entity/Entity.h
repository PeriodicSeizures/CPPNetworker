#pragma once
#include "../engine/Engine.h"
class Entity {
public:
	float x, y;		// position
	float vx, vy;	// velocity
	float ax, ay;	// acceleration

	float terminal_v = 150.f;
	float fric = 4.f;
	bool do_animate = true;
	double angle = 0;

	Engine::Sprite sprite;
	uint8_t cur_frame; // the frame index of the current animation
	uint8_t cur_anim; // modify for user
	uint32_t cur_time; // for timings

public:
	Entity(float x, float y, std::string json_filename);

public:
	virtual void on_render();
	virtual void on_update(float delta);
};

// shared by server and client
//class SharedEntity {
//private:
//
//};

class Player : public Entity {
private:
	//SDL_Texture* halo;
	Engine::Sprite vignette_sprite;

public:
	Player(float x, float y);
	~Player();

	void on_render() override;
	void on_update(float delta) override;

	//void onEvent(SDL_Event& e);
};