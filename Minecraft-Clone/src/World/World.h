

#pragma once

#include "World/Chunk.h"

class World {
public:
	World();

	void SubmitMesh();

private:
	std::vector<Chunk> m_Chunks;
};
