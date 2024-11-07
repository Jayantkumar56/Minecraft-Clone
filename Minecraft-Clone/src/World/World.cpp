


#include "Quirk.h"
#include "World.h"

World::World() {
	m_Chunks.emplace_back();
}

void World::SubmitMesh() {
	Chunk* Front = nullptr;
	Chunk* Back  = nullptr;
	Chunk* Left  = nullptr;
	Chunk* Right = nullptr;

	for (auto& chunk : m_Chunks) {
		ChunkMesh mesh = chunk.GetMesh({Front, Back, Left, Right});
		Quirk::Renderer::SubmitQuadMesh(mesh.Data, mesh.Count);
	}
}

