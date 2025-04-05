

#pragma once

#include "Subchunk.h"

class Chunk {
public:
	Chunk() = default;

	Chunk(glm::i16vec2 chunkPosition) : 
				m_ChunkPosition ( chunkPosition )
	{
		glm::i16vec3 subchunkPos = { m_ChunkPosition.x, 0, m_ChunkPosition.y };
		m_SubChunks.emplace_back(subchunkPos);
	}

	inline auto& GetSubchunkTerrain(int subchunkIndex) {
		return m_SubChunks[subchunkIndex].GetTerrain();
	}

	inline const auto& GetSubchunkTerrain(const int subchunkIndex) const {
		return m_SubChunks[subchunkIndex].GetTerrain();
	}

	inline auto& GetSubchunkMesh(int subchunkIndex) {
		return m_SubChunks[subchunkIndex].GetMesh();
	}

	inline const auto& GetSubchunkMesh(const int subchunkIndex) const {
		return m_SubChunks[subchunkIndex].GetMesh();
	}

private:
	glm::i16vec2 m_ChunkPosition;
	std::vector<SubChunk> m_SubChunks;
};

