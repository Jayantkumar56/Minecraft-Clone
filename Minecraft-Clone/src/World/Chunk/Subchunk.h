

#pragma once


#include "Constants.h"
#include "ChunkMesh.h"
#include "World/Block/Block.h"


class SubChunk {
public:
	SubChunk() :
			m_Blocks(SubChunkBlocksCount)
	{
	}

	inline auto& GetTerrain() noexcept {
		return m_Blocks;
	}

	inline const auto& GetTerrain() const noexcept {
		return m_Blocks;
	}

	inline auto& GetMesh() noexcept {
		return m_Mesh;
	}

	inline const auto& GetMesh() const noexcept {
		return m_Mesh;
	}

private:
	void SetTerrain(std::vector<ChunkBlock>&& terrain) noexcept {
		m_Blocks = std::move(terrain);
	}

private:
	ChunkMesh m_Mesh;
	std::vector<ChunkBlock> m_Blocks;
};

