

#pragma once


#include "Quirk.h"
#include "World/Block/Block.h"
#include "Renderer/Renderer.h"

#include <vector>
#include <array>



constexpr int SubChunkSizeX = 16;
constexpr int SubChunkSizeY = 16;
constexpr int SubChunkSizeZ = 16;

consteval int SubChunkBlocksCount() {
	return SubChunkSizeX * SubChunkSizeY * SubChunkSizeZ;
}

class World;

class SubChunk {
	friend class World;

public:
	SubChunk() {
		GenerateSubChunk();
		GenerateMesh();
	}

	void GenerateSubChunk() {
		ChunkBlock* start = &m_Blocks[0][0][0];
		ChunkBlock* end = start + SubChunkBlocksCount();
		std::fill(start, end, ChunkBlock(BlockId::Grass));
	}

	BlockId GetBlockType(int x, int y, int z) const {
		if (x < 0 || y < 0 || z < 0)
			return BlockId::None;
		if (x >= SubChunkSizeX || y >= SubChunkSizeY || z >= SubChunkSizeZ)
			return BlockId::None;

		return m_Blocks[x][y][z].Id;
	}

	bool IsSolidBlock(int x, int y, int z) const {
		if (x < 0 || y < 0 || z < 0)
			return false;
		if (x >= SubChunkSizeX || y >= SubChunkSizeY || z >= SubChunkSizeZ)
			return false;

		return m_Blocks[x][y][z].Id != BlockId::Air;
	}

	void GenerateMesh();

private:
	ChunkBlock m_Blocks[SubChunkSizeX][SubChunkSizeY][SubChunkSizeZ];
	Mesh m_Mesh;
};

