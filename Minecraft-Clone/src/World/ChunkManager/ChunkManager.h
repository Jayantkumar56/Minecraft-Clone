

#pragma once

#include "World/Chunk/Chunk.h"
#include "Utility/ThreadPool.h"
#include "Player/Player.h"
#include "Renderer/TextureManager.h"
#include "Renderer/Renderer.h"

#include "OwnedChunk.h"

#include <mutex>
#include <shared_mutex>
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

	void RenderChunks(const PerspectiveCamera& camera, TextureManager& textureManager);

	void OnUpdate();

private:
	// must be called under a std::unique_lock on m_ChunksMutex,
	// as taking mesh ownership token for the given chunk
	void SubmitChunkForMeshGeneration(glm::i16vec2 chunkPos);

	// must be called under a std::unique_lock on m_ChunksMutex,
	// as taking terrain ownership token for the given chunk
	void SubmitChunkForTerrainGeneraion(glm::i16vec2 chunkPos);

	bool NeighboursTerrainAvailable(glm::i16vec2 chunkPos) const;

private:
	World* m_World;

	std::unordered_map<glm::i16vec2, OwnedChunk> m_Chunks;
	std::shared_mutex m_ChunksMutex;

	// chunks waiting for their neighbours
	std::vector<glm::i16vec2> m_MeshGenerationQueue;
	std::mutex m_MeshGenerationQueueMutex;

	ThreadPool m_TerrainGeneratorThreadPool;
	ThreadPool m_ChunkMeshGeneratorThreadPool;
};
