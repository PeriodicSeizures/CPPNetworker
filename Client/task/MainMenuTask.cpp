#include "Task.h"

MainMenuTask MAIN_MENU_TASK;

static bool do_stops = false;

MainMenuTask::MainMenuTask() {

	GUITextInput* input = new GUITextInput(15, 20, 2, 2, 21, "address",
		[](std::string &s) {
			size_t at = s.find(':');
			std::string ip = s.substr(0, at);
			std::string port = s.substr(at + 1);

			std::cout << "connect callback() to ip: " << ip << ", port: " << port << "\n";

			if (do_stops) {
				Task::_io_context.stop();
			}

			Task::connection->connect_to_server(Task::_io_context, ip, port);

			Task::cv_run.notify_one();
			do_stops = true;
		}
	);

														// 255 to include \0 char
	GUITextInput* chat = new GUITextInput(15, 600-22, 2, 2, 255, "chat",
		[](std::string& s) {

			std::cout << "chat: " << s << "\n";

			Packet packet;

			if (s.length() < 32) {
				Packet::Chat32 chat;
				std::memcpy(chat.message, s.c_str(), s.length() + 1);
				Task::connection->send_packet(Packet::serialize(chat, chat.type));
			}
			else if (s.length() < 64) {
				Packet::Chat64 chat;
				std::memcpy(chat.message, s.c_str(), s.length() + 1);
				Task::connection->send_packet(Packet::serialize(chat, chat.type));
			}
			else if (s.length() < 128) {
				Packet::Chat128 chat;
				std::memcpy(chat.message, s.c_str(), s.length() + 1);
				Task::connection->send_packet(Packet::serialize(chat, chat.type));
			}
			else if (s.length() < 256) {
				Packet::Chat256 chat;
				std::memcpy(chat.message, s.c_str(), s.length() + 1);
				Task::connection->send_packet(Packet::serialize(chat, chat.type));
			}
			s.clear();
		}
	);

	elements.push_back(input);
	elements.push_back(chat);
}

void MainMenuTask::on_render() {
	for (const auto& element : elements) {
		element->on_render();
	}
}

void MainMenuTask::on_tick() {

}

void MainMenuTask::on_event(SDL_Event& e) {
	// run all events on guis
	for (const auto& element : elements) {
		element->on_event(e);
	}
}