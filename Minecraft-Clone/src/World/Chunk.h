

#pragma once

#include "Subchunk.h"
#include "Core/Renderer/Geometry.h"


struct ChunkMesh {
	Quirk::QuadVertex* Data;
	uint32_t Count;
};

struct AdjacentChunks {
	Chunk* Front;
	Chunk* Back;
	Chunk* Left;
	Chunk* Right;
};

class Chunk {
public:
	Chunk();

	ChunkMesh GetMesh(AdjacentChunks adjChunks);
	inline Subchunk* GetSubchunk(uint16_t index) { return &m_Subchunks[index]; }

private:
	bool m_MeshUpdated;
	std::vector<Quirk::QuadVertex> m_Mesh;
	std::vector<Subchunk> m_Subchunks;
};
