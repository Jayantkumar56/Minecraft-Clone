

#pragma once

#include "Subchunk.h"

enum class ChunkNeighbourLocation {
	PositiveX = 0, NegativeX, PositiveY, NegativeY, NeighboursCount
};

struct ChunkTerrainData {
	ChunkTerrainData() : Data(SubChunkCountInChunk) { 
		for (size_t i = 0; i < Data.size(); ++i) {
			Data[i] = std::vector<ChunkBlock>(SubChunkBlocksCount, ChunkBlock(BlockId::Dirt));
		}
	}

	std::vector<ChunkBlock>& operator[](size_t index) noexcept {
		return Data[index];
	}

private:
	std::vector<std::vector<ChunkBlock>> Data;
};

class Chunk {
	friend class World;
	friend class ChunkManager;
public:
	Chunk() = default;

	Chunk(glm::i16vec2 chunkPosition) : 
				m_ChunkPosition ( chunkPosition )
	{
		glm::i16vec3 subchunkPos = { m_ChunkPosition.x, 0, m_ChunkPosition.y };
		m_SubChunks.emplace_back(subchunkPos);
	}

	void GenerateChunkMesh() {
		for (size_t i = 0; i < m_SubChunks.size(); ++i) {
			m_SubChunks[i].GenerateSubChunkMesh();
		}
	}

private:
	void SetTerrain(ChunkTerrainData&& terrain) {
		for (int i = 0; i < SubChunkCountInChunk; ++i) {
			m_SubChunks[i].SetTerrain( std::move(terrain[i]) );
		}
	}

private:
	glm::i16vec2 m_ChunkPosition;
	std::vector<SubChunk> m_SubChunks;
};

