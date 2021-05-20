#include "Task.h"

MainMenuTask MAIN_MENU_TASK;

void MainMenuTask::on_render() {
	for (auto& element : elements) {
		element.on_render();
	}
}

void MainMenuTask::on_tick() {

}

void MainMenuTask::on_event(SDL_Event& e) {

}