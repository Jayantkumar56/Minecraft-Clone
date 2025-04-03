

#include "ChunkManager.h"
#include "World/World.h"
#include "World/TerrainGenerator/TerrainGenerator.h"
#include "World/ChunkManager/ChunkMeshGenerator.h"

#include <unordered_set>


void ChunkManager::LoadChunksAroundPlayer(glm::vec3 playerPos) {
	uint32_t     renderDistance = RenderDistance;
	glm::i16vec2 playerChunkPos = BlockPositionToChunkPosition(playerPos.x, playerPos.z);

	std::queue<glm::i16vec2> chunkProcessingQueue;
	chunkProcessingQueue.push(playerChunkPos);

	// chunk sequence for terrain generation
	{
		std::scoped_lock lock(m_ChunksMutex, m_ChunkMetaDataMutex);

		while (!chunkProcessingQueue.empty()) {
			glm::i16vec2 chunkPos = chunkProcessingQueue.front();
			chunkProcessingQueue.pop();
			
			if (!m_Chunks.contains(chunkPos)) {
				/*m_Chunks.emplace(
					std::piecewise_construct,
					std::forward_as_tuple(chunkPos.x, chunkPos.y),
					std::forward_as_tuple(chunkPos)
				);*/

				m_Chunks.emplace(chunkPos, Chunk(chunkPos));

				m_ChunkMetaData.emplace(
					std::piecewise_construct,
					std::forward_as_tuple(chunkPos.x, chunkPos.y),
					std::forward_as_tuple(false, false, false, false)
				);

				SubmitChunkForTerrainGeneraion(chunkPos);

				// check for neighbouring chunks
				{
					if (chunkPos.x += 1; std::abs(chunkPos.x - playerChunkPos.x) < renderDistance && !m_Chunks.contains(chunkPos)) {
						chunkProcessingQueue.emplace(chunkPos.x, chunkPos.y);
					}
					if (chunkPos.x -= 2; std::abs(chunkPos.x - playerChunkPos.x) < renderDistance && !m_Chunks.contains(chunkPos)) {
						chunkProcessingQueue.emplace(chunkPos.x, chunkPos.y);
					}

					chunkPos.x += 1;
					if (chunkPos.y += 1; std::abs(chunkPos.y - playerChunkPos.y) < renderDistance && !m_Chunks.contains(chunkPos)) {
						chunkProcessingQueue.emplace(chunkPos.x, chunkPos.y);
					}
					if (chunkPos.y -= 2; std::abs(chunkPos.y - playerChunkPos.y) < renderDistance && !m_Chunks.contains(chunkPos)) {
						chunkProcessingQueue.emplace(chunkPos.x, chunkPos.y);
					}
				}
			}
		}
	}
}

void ChunkManager::SubmitChunkForMeshGeneration(glm::i16vec2 chunkPos) {
	// this thread should execute only after acquiring ownership
	// of the chunk's and it's neighbour's terrain 
	// thus the chunk at chunkPos and it's neighbours 
	// can be accessed without acquiring lock
	m_ChunkMeshGeneratorThreadPool.AddTask([chunkPos, this]() {
		ChunkMeshGenerator::Generate(m_Chunks[chunkPos]);
		std::this_thread::sleep_for(std::chrono::seconds(2));

		std::lock_guard lock(m_ChunkMetaDataMutex);
		ReleaseNeighbourChunkOwnership(chunkPos);

		m_ChunkMetaData[chunkPos].MeshGenerated = true;
		m_ChunkMetaData[chunkPos].HaveMeshOwnership = true;
	});
}

void ChunkManager::SubmitChunkForTerrainGeneraion(glm::i16vec2 chunkPos) {
	m_TerrainGeneratorThreadPool.AddTask([chunkPos, this] {
		// generating terrain
		{
			ChunkTerrainData terrain;
			TerrainGenerator::Generate(chunkPos, terrain);
			std::this_thread::sleep_for(std::chrono::seconds(7));

			std::lock_guard lock(m_ChunksMutex);
			if (!m_Chunks.contains(chunkPos))
				return;

			m_Chunks[chunkPos].SetTerrain(std::move(terrain));
		}

		// submiting for mesh generation
		{
			bool gotNeighbours = false;

			{
				std::lock_guard lock(m_ChunkMetaDataMutex);
				m_ChunkMetaData[chunkPos].TerrainGenerated = true;
				gotNeighbours = GetNeighbourChunkOwnership(chunkPos);
			}

			if (gotNeighbours) {
				SubmitChunkForMeshGeneration(chunkPos);
			}
			else {
				// releasing ownership of the current current chunk
				{
					std::lock_guard lock(m_ChunkMetaDataMutex);

					m_ChunkMetaData[chunkPos].HaveMeshOwnership    = true;
					m_ChunkMetaData[chunkPos].HaveTerrainOwnership = true;
				}

				// make chunk wait for neighbour chunks
				{
					std::lock_guard lock(m_MeshGenerationQueueMutex);
					m_MeshGenerationQueue.emplace_back(chunkPos.x, chunkPos.y);
				}
			}
		}
	});
}

bool ChunkManager::GetNeighbourChunkOwnership(glm::i16vec2 chunkPos) {
	bool result = false;

	bool positiveXNeighbourExist = false;
	bool negativeXNeighbourExist = false;
	bool positiveYNeighbourExist = false;
	bool negativeYNeighbourExist = false;

	bool positiveXNeighbourOwnership = false;
	bool negativeXNeighbourOwnership = false;
	bool positiveYNeighbourOwnership = false;
	bool negativeYNeighbourOwnership = false;

	if (chunkPos.x += 1; m_ChunkMetaData.contains(chunkPos)) {
		positiveXNeighbourExist     = true;
		positiveXNeighbourOwnership = m_ChunkMetaData[chunkPos].HaveTerrainOwnership;
	}
	if (chunkPos.x -= 2; m_ChunkMetaData.contains(chunkPos)) {
		negativeXNeighbourExist     = true;
		negativeXNeighbourOwnership = m_ChunkMetaData[chunkPos].HaveTerrainOwnership;
	}

	chunkPos.x += 1;
	if (chunkPos.y += 1; m_ChunkMetaData.contains(chunkPos)) {
		positiveYNeighbourExist     = true;
		positiveYNeighbourOwnership = m_ChunkMetaData[chunkPos].HaveTerrainOwnership;
	}
	if (chunkPos.y -= 2; m_ChunkMetaData.contains(chunkPos)) {
		negativeYNeighbourExist     = true;
		negativeYNeighbourOwnership = m_ChunkMetaData[chunkPos].HaveTerrainOwnership;
	}



	return true;
}

void ChunkManager::ReleaseNeighbourChunkOwnership(glm::i16vec2 chunkPos) {
	if (chunkPos.x += 1; m_ChunkMetaData.contains(chunkPos)) {
		m_ChunkMetaData[chunkPos].HaveTerrainOwnership = true;
	}
	if (chunkPos.x -= 2; m_ChunkMetaData.contains(chunkPos)) {
		m_ChunkMetaData[chunkPos].HaveTerrainOwnership = true;
	}

	chunkPos.x += 1;
	if (chunkPos.y += 1; m_ChunkMetaData.contains(chunkPos)) {
		m_ChunkMetaData[chunkPos].HaveTerrainOwnership = true;
	}
	if (chunkPos.y -= 2; m_ChunkMetaData.contains(chunkPos)) {
		m_ChunkMetaData[chunkPos].HaveTerrainOwnership = true;
	}
}
