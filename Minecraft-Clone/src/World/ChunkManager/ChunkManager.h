

#pragma once

#include "World/Chunk/Chunk.h"
#include "Utility/ThreadPool.h"
#include "OwnedChunk.h"

#include <shared_mutex>
#include <unordered_map>


namespace std {

	template<>
	struct hash<glm::i16vec2> {

		std::size_t operator()(const glm::i16vec2& value) const {
			return hash<uint32_t>()((static_cast<uint32_t>(value.x) << 16) | static_cast<uint32_t>(value.y));
		}

	};

}

// distance in number of chunks
constexpr uint32_t RenderDistance = 4;

class ChunkManager {
public:
	ChunkManager() :
			m_TerrainGeneratorThreadPool   ( std::thread::hardware_concurrency() >> 1 ),
			m_ChunkMeshGeneratorThreadPool ( std::thread::hardware_concurrency() >> 1 ),
			m_PlayerPos                    ( 0                                        )
	{
	}

	void LoadChunksAroundPlayer(glm::vec3 playerPos);

	void RenderChunks(const ::PerspectiveCamera& camera, TextureManager& textureManager);

	void OnUpdate(const Player& player);

private:
	// must be called under a std::unique_lock on m_ChunksMutex,
	// as taking mesh ownership token for the given chunk
	void SubmitChunkForMeshGeneration(glm::i16vec2 chunkPos);

	// must be called under a std::unique_lock on m_ChunksMutex,
	// as taking terrain ownership token for the given chunk
	void SubmitChunkForTerrainGeneraion(glm::i16vec2 chunkPos);

	// must be called under a std::shared_lock on m_ChunksMutex
	bool NeighboursTerrainAvailable(glm::i16vec2 chunkPos) const;

    void GetChunksToGenerate(glm::i16vec2 prevPlayerChunkPos, glm::i16vec2 currPlayerChunkPos,
                             std::unordered_set<glm::i16vec2>& chunksToGenerate, std::unordered_set<glm::i16vec2>& chunksToUpdate);

    void GetChunksToDelete(glm::i16vec2 prevPlayerChunkPos, glm::i16vec2 currPlayerChunkPos, 
                           std::unordered_set<glm::i16vec2>& chunksToDelete, std::unordered_set<glm::i16vec2>& chunksToUpdate);

    void UpdateChunksAroundPlayer(glm::i16vec2 prevPlayerChunkPos, glm::i16vec2 currPlayerChunkPos);

	// returns an empty view if chunk do not exist or it don't have the ownership
    SubchunkTerrainView GetSubchunkTerrainView(glm::i16vec2 chunkPos, int subchunkIndex) const noexcept;

private:
	std::unordered_map<glm::i16vec2, std::unique_ptr<OwnedChunk>> m_Chunks;
	std::shared_mutex m_ChunksMutex;

	// chunks waiting for their neighbours for mesh generation
	std::unordered_set<glm::i16vec2> m_MeshGenerationQueue;
	std::mutex m_MeshGenerationQueueMutex;

    // chunks waiting for terrain generation
    std::unordered_set<glm::i16vec2> m_TerrainGenerationQueue;
    std::mutex m_TerrainGenerationQueueMutex;

	// chunks waiting to be deleted
	std::unordered_set<glm::i16vec2> m_ChunkDeletionQueue;
	std::mutex m_ChunkDeletionQueueMutex;

	ThreadPool m_TerrainGeneratorThreadPool;
	ThreadPool m_ChunkMeshGeneratorThreadPool;

	// stores player chunkPos
	glm::i16vec2 m_PlayerPos;
};
