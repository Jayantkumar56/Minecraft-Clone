

#pragma once

#include "Subchunk.h"

class Chunk {
	friend class World;
public:
	Chunk() {
		m_SubChunks.emplace_back();
	}

private:
	std::vector<SubChunk> m_SubChunks;
};

