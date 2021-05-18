#include "Packet.h"

uint16_t Packet::S(Type type) {
	return sizes[(unsigned)type];
}