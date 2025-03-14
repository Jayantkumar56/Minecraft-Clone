

#pragma once

#include "Quirk.h"
#include "Block.h"
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
		Block* start = &m_Blocks[0][0][0];
		Block* end   = start + SubChunkBlocksCount();
		std::fill(start, end, Block(BlockType::Grass));
	}

	BlockType GetBlockType(int x, int y, int z) const {
		if (x < 0 || y < 0 || z < 0)
			return BlockType::None;
		if (x >= SubChunkSizeX || y >= SubChunkSizeY || z >= SubChunkSizeZ)
			return BlockType::None;

		return m_Blocks[x][y][z].Type;
	}

	bool IsSolidBlock(int x, int y, int z) const {
		if (x < 0 || y < 0 || z < 0)
			return false;
		if (x >= SubChunkSizeX || y >= SubChunkSizeY || z >= SubChunkSizeZ)
			return false;

		return m_Blocks[x][y][z].Type != BlockType::Air;
	}

	void GenerateMesh();

private:
	Block m_Blocks[SubChunkSizeX][SubChunkSizeY][SubChunkSizeZ];
	Mesh m_Mesh;
};

class Chunk {
	friend class World;
public:
	Chunk() {
		m_SubChunks.emplace_back();
	}

private:
	std::vector<SubChunk> m_SubChunks;
};

