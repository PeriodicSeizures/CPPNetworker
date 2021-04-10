#include "SourcedData.h"

SourcedData::~SourcedData() {
	//delete packet;
	free(this->allocated);
}