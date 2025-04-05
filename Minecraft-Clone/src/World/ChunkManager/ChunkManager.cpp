

#include "ChunkManager.h"
#include "World/World.h"
#include "World/TerrainGenerator/TerrainGenerator.h"
#include "World/ChunkManager/ChunkMeshGenerator.h"

#include <unordered_set>


void ChunkManager::LoadChunksAroundPlayer(glm::vec3 playerPos) {
	uint32_t     renderDistance = RenderDistance;
	glm::i16vec2 playerChunkPos = glm::i16vec2(
		static_cast<uint16_t>(std::floor(playerPos.x / SubChunkSizeX)),
		static_cast<uint16_t>(std::floor(playerPos.z / SubChunkSizeZ))
	);

	std::queue<glm::i16vec2> chunkProcessingQueue;
	chunkProcessingQueue.push(playerChunkPos);

	// chunk sequence for terrain generation
	{
		std::unique_lock lock(m_ChunksMutex);

		while (!chunkProcessingQueue.empty()) {
			glm::i16vec2 chunkPos = chunkProcessingQueue.front();
			chunkProcessingQueue.pop();
			
			if (!m_Chunks.contains(chunkPos)) {
				m_Chunks.emplace(
					std::piecewise_construct,
					std::forward_as_tuple(chunkPos),
					std::forward_as_tuple(chunkPos)
				);

				SubmitChunkForTerrainGeneraion(chunkPos);

				// check for neighbouring chunks
				{
					if (chunkPos.x += 1; (uint16_t)std::abs(chunkPos.x - playerChunkPos.x) < renderDistance && !m_Chunks.contains(chunkPos)) {
						chunkProcessingQueue.emplace(chunkPos.x, chunkPos.y);
					}
					if (chunkPos.x -= 2; (uint16_t)std::abs(chunkPos.x - playerChunkPos.x) < renderDistance && !m_Chunks.contains(chunkPos)) {
						chunkProcessingQueue.emplace(chunkPos.x, chunkPos.y);
					}

					chunkPos.x += 1;
					if (chunkPos.y += 1; (uint16_t)std::abs(chunkPos.y - playerChunkPos.y) < renderDistance && !m_Chunks.contains(chunkPos)) {
						chunkProcessingQueue.emplace(chunkPos.x, chunkPos.y);
					}
					if (chunkPos.y -= 2; (uint16_t)std::abs(chunkPos.y - playerChunkPos.y) < renderDistance && !m_Chunks.contains(chunkPos)) {
						chunkProcessingQueue.emplace(chunkPos.x, chunkPos.y);
					}
				}
			}
		}
	}
}

void ChunkManager::RenderChunks(const PerspectiveCamera& camera, TextureManager& textureManager) {
	CustomRenderer::BeginScene(camera.GetViewProjection(), camera.GetPosition());

	std::shared_lock lock(m_ChunksMutex);

	for (const auto& [chunkPos, chunk] : m_Chunks) {
		if (chunk.HaveMesh()) {
			for (int i = 0; i < SubChunkCountInChunk; ++i) {
				const ChunkMesh& mesh = chunk.GetSubchunkMeshView(i).GetMesh();
				CustomRenderer::SubmitMesh(mesh.GetMesh(), textureManager.GetSpriteSheetTexture());
			}
		}
	}

	CustomRenderer::EndScene();
}

void ChunkManager::OnUpdate() {
	{
		std::unique_lock lock2(m_ChunksMutex, std::defer_lock); // exclusive lock
		std::unique_lock lock1(m_MeshGenerationQueueMutex, std::defer_lock);

		std::lock(lock1, lock2);

		for (size_t i = 0; i < m_MeshGenerationQueue.size(); ++i) {
			SubmitChunkForMeshGeneration(m_MeshGenerationQueue[i]);
		}

		m_MeshGenerationQueue.clear();
	}
}

void ChunkManager::SubmitChunkForMeshGeneration(glm::i16vec2 chunkPos) {
	m_ChunkMeshGeneratorThreadPool.AddTask( std::move(
		[chunkPos, meshToken = std::move(m_Chunks[chunkPos].GetMeshOwnership()), this] () mutable {
			// accessing the current and it's neighbour chunk's terrain as read only for mesh generation
			std::shared_lock chunksLock(m_ChunksMutex);

			if (!meshToken.IsTerrainAvailable() || !NeighboursTerrainAvailable(chunkPos)) {
				chunksLock.unlock();

				std::unique_lock lock(m_MeshGenerationQueueMutex);
				m_MeshGenerationQueue.emplace_back(chunkPos.x, chunkPos.y);

				meshToken.Release();
				return;
			}

			// get terrain views of the neighbours

			// mesh generation code
			{
				const auto& chunk = m_Chunks[chunkPos];

				for (int i = 0; i < SubChunkCountInChunk; ++i) {
					auto& subchunkMesh = meshToken.GetSubchunkMesh(i);
					auto  terrainView  = chunk.GetSubchunkTerrainView(i);

					glm::i16vec3 pos (
						chunkPos.x * SubChunkSizeX,
						i * SubChunkSizeY,
						chunkPos.y * SubChunkSizeZ
					);

					SubChunkMeshGenerator::Generate(subchunkMesh, terrainView, pos);
				}

				meshToken.SetMeshGenerated();
				meshToken.Release();
			}
		}
	));
}

void ChunkManager::SubmitChunkForTerrainGeneraion(glm::i16vec2 chunkPos) {
	m_TerrainGeneratorThreadPool.AddTask( std::move(
		[chunkPos, terrainToken = std::move(m_Chunks[chunkPos].GetTerrainOwnership()), this] () mutable {
			// generating terrain
			{
				// terrain generation code
				for (int i = 0; i < SubChunkCountInChunk; ++i) {
					std::vector<ChunkBlock>& subchunkTerrain = terrainToken.GetSubchunkTerrain(i);

					ChunkBlock* start = &subchunkTerrain[0];
					ChunkBlock* end   = start + SubChunkBlocksCount;
					std::fill(start, end, ChunkBlock(BlockId::CobbleStone));
				}

				terrainToken.SetTerrainGenerated();
				terrainToken.Release();
			}

			std::unique_lock lock(m_ChunksMutex);
			SubmitChunkForMeshGeneration(chunkPos);
		}
	));
}

bool ChunkManager::NeighboursTerrainAvailable(glm::i16vec2 chunkPos) const {
	bool neighboursAvailable = true;

	{
		chunkPos.x += 1;
		auto itr = m_Chunks.find(chunkPos);
		neighboursAvailable = neighboursAvailable && itr != m_Chunks.end() && itr->second.HaveTerrain();
	}

	{
		chunkPos.x -= 2;
		auto itr = m_Chunks.find(chunkPos);
		neighboursAvailable = neighboursAvailable && itr != m_Chunks.end() && itr->second.HaveTerrain();
	}
	
	chunkPos.x += 1;
	{
		chunkPos.y += 1;
		auto itr = m_Chunks.find(chunkPos);
		neighboursAvailable = neighboursAvailable && itr != m_Chunks.end() && itr->second.HaveTerrain();
	}

	{
		chunkPos.y -= 2;
		auto itr = m_Chunks.find(chunkPos);
		neighboursAvailable = neighboursAvailable && itr != m_Chunks.end() && itr->second.HaveTerrain();
	}

	return neighboursAvailable;
}

