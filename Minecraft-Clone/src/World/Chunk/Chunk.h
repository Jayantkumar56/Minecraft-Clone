

#pragma once

#include "Subchunk.h"

class Chunk {
public:
	Chunk() {
		m_SubChunks.emplace_back();
	}

	inline auto& GetSubchunkTerrain(int subchunkIndex) {
		return m_SubChunks[subchunkIndex].GetTerrain();
	}

	inline const auto& GetSubchunkTerrain(const int subchunkIndex) const {
		return m_SubChunks[subchunkIndex].GetTerrain();
	}

	inline auto& GetSubchunkMesh(int subchunkIndex) {
		return m_SubChunks[subchunkIndex].GetMesh();
	}

	inline const auto& GetSubchunkMesh(const int subchunkIndex) const {
		return m_SubChunks[subchunkIndex].GetMesh();
	}

private:
	std::vector<SubChunk> m_SubChunks;
};

