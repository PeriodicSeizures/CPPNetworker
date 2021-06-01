#include "Task.h"

WorldTask WORLD_TASK;

WorldTask::WorldTask() {
	
}

void WorldTask::on_render() {
	// draw the world tiles
	for (float x = 0; x < 28; x++) {
		for (float y = -20; y < 3; y++) {
			//Engine::
			brick_sprite->draw(x * 32.f, y * 32.f, 0);
		}
	}

	// draw entities and world
	player->on_render();

	// render multiplayer entity
	other->on_render();
}

void WorldTask::on_update(float delta) {
	// process game logic
	do_listen();

	player->on_update(delta);
}

void WorldTask::on_event(SDL_Event& e) {

	//switch (e.type) {
	//case SDL_QUIT:
	//	//exit game
	//	break;
	//case SDL_KEYUP:
	//case SDL_KEYDOWN:
	//	player->onEvent(e);
	//default:
	//	break;
	//	
	//	//switch (e.key.keysym.sym) {
	//	//case SDLK_w: // forward
	//	//	player->
	//	//case SDLK_ESCAPE: // open pause menu
	//	//case SDLK_SLASH: // open command menu
	//	//	break;
	//	//	//COMMAND_TASK.focus();
	//	//}
	//}
}

void WorldTask::do_listen() {

	AsyncQueue<Packet>& in = Task::connection->in_packets;

	uint16_t processed = 10;
	while (in.count() > 1 && processed--) {

		Packet packet = in.pop_front();

		switch (packet.type) {
		case Packet::Type::SRC_CLIENT_TRUSTED_MOTION: {
			Packet::TrustedMotion* motion = Packet::deserialize<Packet::TrustedMotion>(packet);
			other->x = motion->x;
			other->y = motion->y;
			other->vx = motion->vx;
			other->vy = motion->vy;
			other->ax = motion->ax;
			other->ay = motion->ay;
			break;
		}
		}

	}

	
}