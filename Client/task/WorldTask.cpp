#include "Task.h"

WorldTask WORLD_TASK;

WorldTask::WorldTask() {
	
}

void WorldTask::on_render() {
	// draw the world tiles
	for (float x = 0; x < 25; x++) {
		for (float y = -20; y < 0; y++) {
			//Engine::
			brick_sprite->draw(x * 32.f, y * 32.f, 0, 0, 0);
		}
	}

	// draw entities and world
	player->on_render();

}

void WorldTask::on_update(float delta) {
	// process game logic
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