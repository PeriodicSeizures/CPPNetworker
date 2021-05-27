#include "Task.h"

WorldTask WORLD_TASK;

WorldTask::WorldTask() {

}

void WorldTask::on_render() {
	// draw entities and world
	player->onRender();
}

void WorldTask::on_tick() {
	// process game logic
}

void WorldTask::on_event(SDL_Event& e) {
	switch (e.type) {
	case SDL_QUIT:
		//exit game
		break;
	case SDL_KEYDOWN: {

		switch (e.key.keysym.sym) {
		case SDLK_ESCAPE:
			// open pause menu
		case SDLK_SLASH:
			// open command menu
			COMMAND_TASK.focus();
		}
	}
	}
}