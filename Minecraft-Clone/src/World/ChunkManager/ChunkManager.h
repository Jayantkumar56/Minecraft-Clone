

#pragma once

#include "World/Chunk/Chunk.h"
#include "Utility/ThreadPool.h"
#include "Player/Player.h"
#include "Renderer/TextureManager.h"
#include "Renderer/Renderer.h"

#include <mutex>
#include <unordered_map>
#include <glm/glm.hpp>

namespace std {

	template<>
	struct hash<glm::i16vec2> {

		std::size_t operator()(const glm::i16vec2& value) const {
			return hash<uint32_t>()((static_cast<uint32_t>(value.x) << 16) | static_cast<uint32_t>(value.y));
		}

	};

}

struct ChunkMetaData {
	bool MeshGenerated    = false;
	bool TerrainGenerated = false;
	bool HaveMeshOwnership    = false;
	bool HaveTerrainOwnership = false;
};

class World;

// distance in number of chunks
constexpr uint32_t RenderDistance = 5;

class ChunkManager {
public:
	ChunkManager(World* world) : 
			m_World                        ( world                                    ),
			m_TerrainGeneratorThreadPool   ( std::thread::hardware_concurrency() >> 1 ),
			m_ChunkMeshGeneratorThreadPool ( std::thread::hardware_concurrency() >> 1 )
	{
	}

	void LoadChunksAroundPlayer(glm::vec3 playerPos);

	void RenderChunks(const PerspectiveCamera& camera, TextureManager& textureManager) {
		CustomRenderer::BeginScene(camera.GetViewProjection(), camera.GetPosition());

		std::scoped_lock lock(m_ChunksMutex, m_ChunkMetaDataMutex);
		auto& chunks = m_Chunks;
		for (auto& [chunkPos, chunk] : chunks) {
			if (m_ChunkMetaData[chunkPos].MeshGenerated && m_ChunkMetaData[chunkPos].HaveMeshOwnership) {
				for (auto& subchunk : chunk.m_SubChunks) {
					CustomRenderer::SubmitMesh(subchunk.m_Mesh.GetMesh(), textureManager.GetSpriteSheetTexture());
				}
			}
		}

		CustomRenderer::EndScene();
	}

	inline glm::i16vec2 BlockPositionToChunkPosition(float x, float z) const {
		return glm::i16vec2 (
			static_cast<uint16_t>(std::floor(x / SubChunkSizeX)),
			static_cast<uint16_t>(std::floor(z / SubChunkSizeZ))
		);
	}

private:
	void SubmitChunkForMeshGeneration(glm::i16vec2 chunkPos);
	void SubmitChunkForTerrainGeneraion(glm::i16vec2 chunkPos);

	bool GetNeighbourChunkOwnership(glm::i16vec2 chunkPos);
	void ReleaseNeighbourChunkOwnership(glm::i16vec2 chunkPos);

private:
	World* m_World;

	// chunk position is different from block position
	std::unordered_map<glm::i16vec2, Chunk> m_Chunks;
	std::mutex m_ChunksMutex;

	std::unordered_map<glm::i16vec2, ChunkMetaData> m_ChunkMetaData;
	std::mutex m_ChunkMetaDataMutex;

	// chunks waiting for their neighbours
	std::vector<glm::i16vec2> m_MeshGenerationQueue;
	std::mutex m_MeshGenerationQueueMutex;

	ThreadPool m_TerrainGeneratorThreadPool;
	ThreadPool m_ChunkMeshGeneratorThreadPool;
};
