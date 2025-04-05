

#pragma once

#include "World/Chunk/Subchunk.h"

class SubchunkTerrainView {
public:
	SubchunkTerrainView(const std::vector<ChunkBlock>& blocks) noexcept :
			m_Blocks(blocks)
	{
	}

	inline const auto& GetBlocks() const noexcept {
		return m_Blocks; 
	}

	inline ChunkBlock GetBlock(uint32_t x, uint32_t y, uint32_t z) const {
		QK_ASSERT(x < SubChunkSizeX && y < SubChunkSizeY && z < SubChunkSizeZ, "Block index out of bound!");
		return m_Blocks[static_cast<size_t>((z * SubChunkSizeX * SubChunkSizeY) + (y * SubChunkSizeX) + x)];
	}

	inline BlockId GetBlockType(int x, int y, int z) const {
		if (x < 0 || y < 0 || z < 0)
			return BlockId::None;
		if (x >= SubChunkSizeX || y >= SubChunkSizeY || z >= SubChunkSizeZ)
			return BlockId::None;

		return GetBlock(x, y, z).Id;
	}

	inline bool IsSolidBlock(int x, int y, int z) const {
		if (x < 0 || y < 0 || z < 0)
			return false;
		if (x >= SubChunkSizeX || y >= SubChunkSizeY || z >= SubChunkSizeZ)
			return false;

		return GetBlock(x, y, z).Id != BlockId::Air;
	}

private:
	const std::vector<ChunkBlock>& m_Blocks;
};


class SubchunkMeshView {
public:
	SubchunkMeshView(const ChunkMesh& mesh) noexcept :
			m_Mesh(mesh)
	{
	}

	const auto& GetMesh() const noexcept {
		return m_Mesh;
	}

private:
	const ChunkMesh& m_Mesh;
};
