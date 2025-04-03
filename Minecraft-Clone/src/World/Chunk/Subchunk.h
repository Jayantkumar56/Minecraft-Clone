

#pragma once


#include "Constants.h"
#include "ChunkMesh.h"
#include "World/Block/Block.h"
#include "World/ChunkManager/ChunkMeshGenerator.h"

#include <vector>
#include <array>



class SubChunk {
	friend class ChunkManager;
	friend class Chunk;

public:
	SubChunk(glm::i16vec3 subchunkPosition) :
			m_SubChunkPosition ( subchunkPosition        ), 
			m_MeshGenerator    ( *this, m_Mesh, m_Blocks ) 
	{
	}

	void GenerateSubChunkMesh() {
		ChunkBlock* start = &m_Blocks[0];
		ChunkBlock* end   = start + SubChunkBlocksCount;
		std::fill(start, end, ChunkBlock(BlockId::Dirt));

		m_MeshGenerator.GenerateMesh();
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

	inline glm::i16vec3 GetChunkPosition() const noexcept { 
		return m_SubChunkPosition;
	}

	inline glm::i16vec3 GetBlockPosition() const noexcept {
		return glm::i16vec3(
			m_SubChunkPosition.x * SubChunkSizeX,
			m_SubChunkPosition.y * SubChunkSizeY,
			m_SubChunkPosition.z * SubChunkSizeZ
		);
	}

private:
	void SetTerrain(std::vector<ChunkBlock>&& terrain) noexcept {
		m_Blocks = std::move(terrain);
	}

private:
	glm::i16vec3 m_SubChunkPosition;
	//std::array<ChunkBlock, SubChunkBlocksCount> m_Blocks;
	std::vector<ChunkBlock> m_Blocks;
	ChunkMesh             m_Mesh;
	SubChunkMeshGenerator m_MeshGenerator;
};

