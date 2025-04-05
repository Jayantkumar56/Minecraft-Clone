

#pragma once


#include "Constants.h"
#include "ChunkMesh.h"
#include "World/Block/Block.h"
#include "World/ChunkManager/ChunkMeshGenerator.h"

#include <vector>
#include <array>



class SubChunk {
public:
	SubChunk(glm::i16vec3 subchunkPosition) :
			m_SubChunkPosition(subchunkPosition),
			m_Blocks(SubChunkBlocksCount)
	{
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
	glm::i16vec3 m_SubChunkPosition;
	std::vector<ChunkBlock> m_Blocks;
	ChunkMesh             m_Mesh;
};

