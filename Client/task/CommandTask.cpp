#include "Task.h"

CommandTask COMMAND_TASK;

void CommandTask::on_render() {
	input.on_render();
}

void CommandTask::on_tick() {

}

void CommandTask::on_event(SDL_Event& e) {
	if (e.type == SDL_KEYDOWN) {
		if (e.key.keysym.sym == SDLK_RETURN) {
			input.text.clear();
			WORLD_TASK.focus();
			return;
		}
	}
	input.on_event(e);
}