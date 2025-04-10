

#pragma once

#include "World/Chunk/Chunk.h"
#include "ChunkOwnershipTokens.h"
#include "SubchunkView.h"


class OwnedChunk {
	friend class TerrainOwnershipToken<OwnedChunk>;
	friend class MeshOwnershipToken<OwnedChunk>;
public:
	OwnedChunk() = default;

	inline TerrainOwnershipToken<OwnedChunk> GetTerrainOwnership() noexcept {
		OwnedChunk* ptr = nullptr;
		if (m_HaveTerrainOwnership) {
			ptr = this;
			m_HaveTerrainOwnership = false;

			// if some thread have terrain ownership for write we cannot delete the chunk
			++m_NotDeleteableRequests;
		}

		return TerrainOwnershipToken<OwnedChunk>(ptr);
	}

	inline MeshOwnershipToken<OwnedChunk> GetMeshOwnership() noexcept {
		OwnedChunk* ptr = nullptr;
		if (m_HaveMeshOwnership) {
			ptr = this;
			m_HaveMeshOwnership = false;

			// if some thread have mesh ownership for write we cannot delete the chunk
			++m_NotDeleteableRequests;
		}

		return MeshOwnershipToken<OwnedChunk>(ptr);
	}

	inline auto GetSubchunkTerrainView(const int subchunkIndex) const {
		const std::vector<ChunkBlock>* blocks = HaveTerrain() ? &m_Chunk.GetSubchunkTerrain(subchunkIndex) : nullptr;
		return SubchunkTerrainView(blocks);
	}

	inline auto GetSubchunkMeshView(const int subchunkIndex) const {
		const ChunkMesh* mesh = HaveMesh() ? &m_Chunk.GetSubchunkMesh(subchunkIndex) : nullptr;
		return SubchunkMeshView(mesh);
	}

	inline bool HaveMesh()    const noexcept { return m_MeshGenerated && m_HaveMeshOwnership;       }
	inline bool HaveTerrain() const noexcept { return m_TerrainGenerated && m_HaveTerrainOwnership; }
	inline bool IsDeletable() const noexcept { return m_NotDeleteableRequests == 0;                 }
	
private:
	inline void ResetTerrainOwnership() noexcept {
		// rstoring the deletable counter
		--m_NotDeleteableRequests;

		m_HaveTerrainOwnership = true;
	}

	inline void ResetMeshOwnership() noexcept {
		// rstoring the deletable counter
		--m_NotDeleteableRequests;

		m_HaveMeshOwnership = true;
	}

	inline auto& GetSubchunkTerrain(int subchunkIndex) { return m_Chunk.GetSubchunkTerrain(subchunkIndex); }
	inline auto& GetSubchunkMesh(int subchunkIndex)    { return m_Chunk.GetSubchunkMesh(subchunkIndex);    }

	inline void SetTerrainGenerated() noexcept { m_TerrainGenerated = true; }
	inline void SetMeshGenerated()    noexcept { m_MeshGenerated = true;    }

private:
	Chunk m_Chunk;
	bool m_MeshGenerated        = false;
	bool m_TerrainGenerated     = false;
	std::atomic_bool m_HaveMeshOwnership    = true;
	std::atomic_bool m_HaveTerrainOwnership = true;
	std::atomic_int m_NotDeleteableRequests = 0;
};
