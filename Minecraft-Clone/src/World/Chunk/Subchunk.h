

#pragma once


#include "ChunkMesh.h"
#include "ChunkMeshGenerator.h"
#include "World/Block/Block.h"

#include <vector>
#include <array>


class World;

class SubChunk {
	friend class World;

public:
	SubChunk() : m_MeshGenerator(*this, m_Mesh, m_Blocks) {
		GenerateSubChunk();
		m_MeshGenerator.GenerateMesh();
	}

	void GenerateSubChunk() {
		ChunkBlock* start = &m_Blocks[0];
		ChunkBlock* end = start + SubChunkBlocksCount;
		std::fill(start, end, ChunkBlock(BlockId::CobbleStone));
	}

	BlockId GetBlockType(int x, int y, int z) const {
		if (x < 0 || y < 0 || z < 0)
			return BlockId::None;
		if (x >= SubChunkSizeX || y >= SubChunkSizeY || z >= SubChunkSizeZ)
			return BlockId::None;

		return GetBlock(x, y, z).Id;
	}

	bool IsSolidBlock(int x, int y, int z) const {
		if (x < 0 || y < 0 || z < 0)
			return false;
		if (x >= SubChunkSizeX || y >= SubChunkSizeY || z >= SubChunkSizeZ)
			return false;

		return GetBlock(x, y, z).Id != BlockId::Air;
	}

	inline ChunkBlock GetBlock(uint32_t x, uint32_t y, uint32_t z) const {
		QK_ASSERT(x < SubChunkSizeX && y < SubChunkSizeY && z < SubChunkSizeZ, "Block index out of bound!");
		return m_Blocks[static_cast<size_t>((z * SubChunkSizeX * SubChunkSizeY) + (y * SubChunkSizeX) + x)];
	}

private:
	std::array<ChunkBlock, SubChunkBlocksCount> m_Blocks;
	ChunkMesh             m_Mesh;
	SubChunkMeshGenerator m_MeshGenerator;
};

