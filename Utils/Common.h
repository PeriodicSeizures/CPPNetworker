#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

typedef size_t UUID;

// DEPRECATED
enum class Direction : uint16_t {
	UP, UP_RIGHT, RIGHT, DOWN_RIGHT, DOWN, DOWN_LEFT, LEFT, UP_LEFT,
	HALT
};

enum class Input : uint16_t {
	PRESS_UP, PRESS_RIGHT, PRESS_DOWN, PRESS_LEFT,
	RELEASE_UP, RELEASE_RIGHT, RELEASE_DOWN, RELEASE_LEFT,
	SHOOT
};

#endif
