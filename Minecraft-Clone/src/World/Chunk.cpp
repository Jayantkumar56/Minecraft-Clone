


#include "Chunk.h"

Chunk::Chunk() {
	m_MeshUpdated = true;
	m_Subchunks.emplace_back();
	m_Mesh.reserve(1000);
}

ChunkMesh Chunk::GetMesh(AdjacentChunks adjChunks) {
	if (m_MeshUpdated) {
		Subchunk* front  = nullptr;
		Subchunk* back   = nullptr;
		Subchunk* top    = nullptr;
		Subchunk* bottom = nullptr;
		Subchunk* left   = nullptr;
		Subchunk* right  = nullptr;

		for (size_t i = 0; i < m_Subchunks.size(); ++i) {
			top   = (i == m_Subchunks.size()) ? nullptr : &m_Subchunks[i + 1];
			front = (adjChunks.Front == NULL) ? nullptr : adjChunks.Front->GetSubchunk(i);
			back  = (adjChunks.Back  == NULL) ? nullptr : adjChunks.Back->GetSubchunk(i);
			left  = (adjChunks.Left  == NULL) ? nullptr : adjChunks.Left->GetSubchunk(i);
			right = (adjChunks.Right == NULL) ? nullptr : adjChunks.Right->GetSubchunk(i);

			m_Subchunks[i].CreateMesh(m_Mesh, { front, back, top, bottom, left, right });

			bottom = &m_Subchunks[i];
		}
	}

	return { m_Mesh.data(), static_cast<uint32_t>(m_Mesh.size()) };
}
