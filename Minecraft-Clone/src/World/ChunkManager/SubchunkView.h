

#pragma once

#include "World/Chunk/Subchunk.h"
#include "Utility/Math.h"

class SubchunkTerrainView {
public:
	SubchunkTerrainView(const std::vector<ChunkBlock>* blocks) noexcept :
			m_Blocks(blocks)
	{
	}

	SubchunkTerrainView(const SubchunkTerrainView& other) noexcept :
			m_Blocks(other.m_Blocks)
	{
	}

	inline ChunkBlock GetBlock(uint32_t x, uint32_t y, uint32_t z) const {
		if (m_Blocks == nullptr) return ChunkBlock();
		
		QK_ASSERT(x < SubChunkSizeX && y < SubChunkSizeY && z < SubChunkSizeZ, "Block index out of bound!");
		return (*m_Blocks)[static_cast<size_t>((z * SubChunkSizeX * SubChunkSizeY) + (y * SubChunkSizeX) + x)];
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
	
	inline const bool                     IsValid()   const noexcept { return m_Blocks != nullptr; }
	inline const std::vector<ChunkBlock>& GetBlocks() const noexcept { return *m_Blocks;           }

private:
	const std::vector<ChunkBlock>* m_Blocks;
};


class SubchunkMeshView {
public:
	SubchunkMeshView(const ChunkMesh* mesh) noexcept :
			m_Mesh(mesh)
	{
	}

	const ChunkMesh& GetMesh() const noexcept { return *m_Mesh;           }
	const bool       IsValid() const noexcept { return m_Mesh != nullptr; }

private:
	const ChunkMesh* m_Mesh;
};


class SubchunkTerrainViewEx {
	enum TerrainIndex : int32_t {
		NegativeZ  =  -3,
		NegativeY  =  -2,
		NegativeX  =  -1,
		Center     =   0,
		PositiveX  =   1,
		PositiveY  =   2,
		PositiveZ  =   3
	};

public:
    SubchunkTerrainViewEx(
		SubchunkTerrainView current,
		SubchunkTerrainView positiveXNeighbourTerrain,
		SubchunkTerrainView negativeXNeighbourTerrain,
		SubchunkTerrainView positiveYNeighbourTerrain,
		SubchunkTerrainView negativeYNeighbourTerrain,
		SubchunkTerrainView positiveZNeighbourTerrain,
		SubchunkTerrainView negativeZNeighbourTerrain
	) :
			m_Current(current),
			m_PositiveXNeighbourTerrain(positiveXNeighbourTerrain),
			m_NegativeXNeighbourTerrain(negativeXNeighbourTerrain),
			m_PositiveYNeighbourTerrain(positiveYNeighbourTerrain),
			m_NegativeYNeighbourTerrain(negativeYNeighbourTerrain),
			m_PositiveZNeighbourTerrain(positiveZNeighbourTerrain),
			m_NegativeZNeighbourTerrain(negativeZNeighbourTerrain)
	{
	}

	inline ChunkBlock GetBlock(int32_t posx, int32_t posy, int32_t posz) const {
		int32_t x = (posx + SubChunkSizeX) % SubChunkSizeX;
		int32_t y = (posy + SubChunkSizeY) % SubChunkSizeY;
		int32_t z = (posz + SubChunkSizeZ) % SubChunkSizeZ;

		int32_t indexX = Math::FloorDiv(posx, SubChunkSizeX) * 1;
		int32_t indexY = Math::FloorDiv(posy, SubChunkSizeY) * 2;
		int32_t indexZ = Math::FloorDiv(posz, SubChunkSizeZ) * 3;

		int32_t index = indexX + indexY + indexZ;

		return GetTerrain(static_cast<TerrainIndex>(index))->GetBlock(x, y, z);
	}

	inline bool IsSolidBlock(int x, int y, int z) const {
		return GetBlock(x, y, z).Id != BlockId::Air;
	}

private:
	inline const SubchunkTerrainView* GetTerrain(TerrainIndex index) const {
		switch (index) {
			case TerrainIndex::NegativeZ: return &m_NegativeZNeighbourTerrain;
			case TerrainIndex::NegativeY: return &m_NegativeYNeighbourTerrain;
			case TerrainIndex::NegativeX: return &m_NegativeXNeighbourTerrain;
			case TerrainIndex::Center:    return &m_Current;
			case TerrainIndex::PositiveX: return &m_PositiveXNeighbourTerrain;
			case TerrainIndex::PositiveY: return &m_PositiveYNeighbourTerrain;
			case TerrainIndex::PositiveZ: return &m_PositiveZNeighbourTerrain;
		}
		return nullptr;
	}

private:
	SubchunkTerrainView m_Current;
	SubchunkTerrainView m_PositiveXNeighbourTerrain;
	SubchunkTerrainView m_NegativeXNeighbourTerrain;
	SubchunkTerrainView m_PositiveYNeighbourTerrain;
	SubchunkTerrainView m_NegativeYNeighbourTerrain;
	SubchunkTerrainView m_PositiveZNeighbourTerrain;
	SubchunkTerrainView m_NegativeZNeighbourTerrain;
};
