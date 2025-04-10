

#include "World/ChunkManager/ChunkMeshGenerator.h"
#include "World/TerrainGenerator/TerrainGenerator.h"

#include "Player/Player.h"
#include "Renderer/TextureManager.h"
#include "Renderer/Renderer.h"

#include "ChunkManager.h"

#include <iostream>


void ChunkManager::LoadChunksAroundPlayer(glm::vec3 playerPos) {
	glm::i16vec2 currPlayerChunkPos = glm::i16vec2(
		static_cast<uint16_t>(std::floor(playerPos.x / SubChunkSizeX)),
		static_cast<uint16_t>(std::floor(playerPos.z / SubChunkSizeZ))
	);

    UpdateChunksAroundPlayer(m_PlayerPos, currPlayerChunkPos);
	m_PlayerPos = currPlayerChunkPos;
}

void ChunkManager::RenderChunks(const PerspectiveCamera& camera, TextureManager& textureManager) {
	CustomRenderer::BeginScene(camera.GetViewProjection(), camera.GetPosition());

	std::shared_lock lock(m_ChunksMutex);

	for (const auto& [chunkPos, chunk] : m_Chunks) {
		if (chunk->HaveMesh()) {
			for (int i = 0; i < SubChunkCountInChunk; ++i) {
				const ChunkMesh& mesh = chunk->GetSubchunkMeshView(i).GetMesh();
				CustomRenderer::SubmitMesh(mesh.GetMesh(), textureManager.GetSpriteSheetTexture());
			}
		}
	}

	CustomRenderer::EndScene();
}

void ChunkManager::OnUpdate(const Player& player) {
	// checking for payer movement and generating new chunks or deleting chunks out of render distance
	{
		glm::vec3 currPlayerPos{ player.GetPosition() };

		glm::i16vec2 currPlayerChunkPos{
			static_cast<uint16_t>(std::floor(currPlayerPos.x / SubChunkSizeX)),
			static_cast<uint16_t>(std::floor(currPlayerPos.z / SubChunkSizeZ))
		};

		if (currPlayerChunkPos != m_PlayerPos) {
            UpdateChunksAroundPlayer(m_PlayerPos, currPlayerChunkPos);
            m_PlayerPos = currPlayerChunkPos;
		}
	}

    // processing chunks to delete
    {
        std::unique_lock chunklock(m_ChunksMutex, std::defer_lock);
        std::unique_lock chunkDeletionQueuelock(m_ChunkDeletionQueueMutex, std::defer_lock);

        std::lock(chunklock, chunkDeletionQueuelock);

        if (m_ChunkDeletionQueue.size() != 0) {
            std::unordered_set<glm::i16vec2> newQueue;
            newQueue.reserve(m_ChunkDeletionQueue.size());

            for (const auto& chunkPos : m_ChunkDeletionQueue) {
                if (m_Chunks.at(chunkPos)->IsDeletable()) {
                    m_Chunks.erase(chunkPos);
                }
                else {
                    newQueue.emplace(chunkPos);
                }
            }

            m_ChunkDeletionQueue = std::move(newQueue);
        }
    }

    // submiting chunks for terrain generation
    {
        std::unique_lock chunksLock(m_ChunksMutex, std::defer_lock);
        std::unique_lock terrainGenerationQueuelock(m_TerrainGenerationQueueMutex, std::defer_lock);

        std::lock(chunksLock, terrainGenerationQueuelock);

        if (m_TerrainGenerationQueue.size() != 0) {
            for (auto& chunkPos : m_TerrainGenerationQueue) {
                m_Chunks.emplace(std::piecewise_construct, std::forward_as_tuple(chunkPos.x, chunkPos.y), std::forward_as_tuple(new OwnedChunk()));
                SubmitChunkForTerrainGeneraion(chunkPos);
            }

            m_TerrainGenerationQueue.clear();
        }
    }

	// pushing queued chunks for mesh generation 
	{
		std::unique_lock chunklock(m_ChunksMutex, std::defer_lock);
		std::unique_lock meshGenerationQueuelock(m_MeshGenerationQueueMutex, std::defer_lock);

		std::lock(chunklock, meshGenerationQueuelock);

        if (m_MeshGenerationQueue.size() != 0) {
            for (auto& chunkPos : m_MeshGenerationQueue) {
                SubmitChunkForMeshGeneration(chunkPos);
            }

            m_MeshGenerationQueue.clear();
        }
	}
}

void ChunkManager::SubmitChunkForMeshGeneration(glm::i16vec2 chunkPos) {
	m_ChunkMeshGeneratorThreadPool.AddTask( std::move(
        [chunkPos, meshToken = std::move(m_Chunks[chunkPos]->GetMeshOwnership()), this]() mutable {
            // accessing the current and it's neighbour chunk's terrain as read only for mesh generation
            std::shared_lock chunksLock(m_ChunksMutex);

            if (!meshToken.HaveOwnership() || !meshToken.IsTerrainAvailable() || !NeighboursTerrainAvailable(chunkPos)) {
                chunksLock.unlock();

                std::unique_lock lock(m_MeshGenerationQueueMutex);
                m_MeshGenerationQueue.emplace(chunkPos.x, chunkPos.y);

                meshToken.Release();
                return;
            }

			glm::i16vec2 positiveXNeighbourPos (chunkPos.x +1, chunkPos.y   );
			glm::i16vec2 negativeXNeighbourPos (chunkPos.x -1, chunkPos.y   );
			glm::i16vec2 positiveZNeighbourPos (chunkPos.x,    chunkPos.y +1);
			glm::i16vec2 negativeZNeighbourPos (chunkPos.x,    chunkPos.y -1);

			// mesh generation code
			{
				const auto& chunk = m_Chunks[chunkPos];

				for (int i = 0; i < SubChunkCountInChunk; ++i) {
					auto& subchunkMesh = meshToken.GetSubchunkMesh(i);
					auto  terrainView  = chunk->GetSubchunkTerrainView(i);

					glm::i16vec3 pos (
						chunkPos.x * SubChunkSizeX,
						i * SubChunkSizeY,
						chunkPos.y * SubChunkSizeZ
					);

					SubchunkTerrainViewEx terrain (
						GetSubchunkTerrainView ( chunkPos,              i    ),
						GetSubchunkTerrainView ( positiveXNeighbourPos, i    ),
						GetSubchunkTerrainView ( negativeXNeighbourPos, i    ),
						GetSubchunkTerrainView ( chunkPos,              i +1 ),
						GetSubchunkTerrainView ( chunkPos,              i -1 ),
						GetSubchunkTerrainView ( positiveZNeighbourPos, i    ),
						GetSubchunkTerrainView ( negativeZNeighbourPos, i    )
					);

					SubChunkMeshGenerator generator(pos, meshToken.GetSubchunkMesh(i), terrain);
					generator.Generate();
				}

				meshToken.SetMeshGenerated();
				meshToken.Release();
			}
		}
	));
}

void ChunkManager::SubmitChunkForTerrainGeneraion(glm::i16vec2 chunkPos) {
	m_TerrainGeneratorThreadPool.AddTask( std::move(
		[chunkPos, terrainToken = std::move(m_Chunks[chunkPos]->GetTerrainOwnership()), this] () mutable {
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

    glm::i16vec2 pos = chunkPos;

	{
		chunkPos.x += 1;
		auto itr = m_Chunks.find(chunkPos);

		if (itr != m_Chunks.end()) {
			neighboursAvailable = neighboursAvailable && itr->second->HaveTerrain();
		}
	}

	{
		chunkPos.x -= 2;
		auto itr = m_Chunks.find(chunkPos);

		if (itr != m_Chunks.end()) {
			neighboursAvailable = neighboursAvailable && itr->second->HaveTerrain();
		}
	}
	
	chunkPos.x += 1;
	{
		chunkPos.y += 1;
		auto itr = m_Chunks.find(chunkPos);

		if (itr != m_Chunks.end()) {
			neighboursAvailable = neighboursAvailable && itr->second->HaveTerrain();
		}
	}

	{
		chunkPos.y -= 2;
		auto itr = m_Chunks.find(chunkPos);

		if (itr != m_Chunks.end()) {
			neighboursAvailable = neighboursAvailable && itr->second->HaveTerrain();
		}
	}

	return neighboursAvailable;
}

void ChunkManager::GetChunksToGenerate(glm::i16vec2 prevPlayerChunkPos, glm::i16vec2 currPlayerChunkPos, 
                                       std::unordered_set<glm::i16vec2>& chunksToGenerate, std::unordered_set<glm::i16vec2>& chunksToUpdate)
{
    glm::i16vec2 direction            { currPlayerChunkPos - m_PlayerPos };
    glm::i16vec2 firstChunkToGenerate { direction * (RenderDistance - 1) };
    firstChunkToGenerate += currPlayerChunkPos;

    // when first loading the world or plyer is teleported
    if (prevPlayerChunkPos == currPlayerChunkPos) {
        firstChunkToGenerate = currPlayerChunkPos;
    }

    int32_t renderDistance = RenderDistance;
    std::queue<glm::i16vec2> chunkProcessingQueue;

    constexpr int noOfNeighbours = 4;
    glm::i16vec2  neighbours[noOfNeighbours]{};
    glm::i16vec2  neighboursRelativePos[noOfNeighbours]{};

    // holding shared lock on the m_Chunks
    std::shared_lock chunksLock(m_ChunksMutex);

    // adding first chunk to generate in the list
    {
        if (!m_Chunks.contains(firstChunkToGenerate)) {
            chunkProcessingQueue.emplace(firstChunkToGenerate);
            chunksToGenerate.emplace(firstChunkToGenerate);
        }
    }

    while (!chunkProcessingQueue.empty()) {
        glm::i16vec2 chunkPos = chunkProcessingQueue.front();
        chunkProcessingQueue.pop();

        neighbours[0].x = chunkPos.x +1;                neighbours[0].y = chunkPos.y;
        neighbours[1].x = chunkPos.x -1;                neighbours[1].y = chunkPos.y;
        neighbours[2].x = chunkPos.x;                   neighbours[2].y = chunkPos.y +1;
        neighbours[3].x = chunkPos.x;                   neighbours[3].y = chunkPos.y -1;

        neighboursRelativePos[0] = neighbours[0] - currPlayerChunkPos;
        neighboursRelativePos[1] = neighbours[1] - currPlayerChunkPos;
        neighboursRelativePos[2] = neighbours[2] - currPlayerChunkPos;
        neighboursRelativePos[3] = neighbours[3] - currPlayerChunkPos;

        for (int i = 0; i < noOfNeighbours; ++i) {
            if (std::abs(neighboursRelativePos[i].x) < renderDistance && std::abs(neighboursRelativePos[i].y) < RenderDistance) {
                if (m_Chunks.contains(neighbours[i])) {
                    if (!chunksToUpdate.contains(neighbours[i])) {
                        chunksToUpdate.emplace(neighbours[i]);
                    }
                }
                else if (!chunksToGenerate.contains(neighbours[i])) {
                    chunkProcessingQueue.emplace(neighbours[i]);
                    chunksToGenerate.emplace(neighbours[i]);
                }
            }
        }
    }
}

void ChunkManager::GetChunksToDelete(glm::i16vec2 prevPlayerChunkPos, glm::i16vec2 currPlayerChunkPos, 
                                     std::unordered_set<glm::i16vec2>& chunksToDelete, std::unordered_set<glm::i16vec2>& chunksToUpdate)
{
    glm::i16vec2 direction          { prevPlayerChunkPos - currPlayerChunkPos };
    glm::i16vec2 firstChunkToDelete { direction * RenderDistance              };
    firstChunkToDelete += currPlayerChunkPos;

    int32_t renderDistance = RenderDistance;
    std::queue<glm::i16vec2> chunkProcessingQueue;

    constexpr int noOfNeighbours = 4;
    glm::i16vec2  neighbours[noOfNeighbours]{};
    glm::i16vec2  neighboursRelativePos[noOfNeighbours]{};

    // holding shared lock on the m_Chunks
    std::shared_lock chunksLock(m_ChunksMutex);

    // adding first chunk to delete in the list
    {
        if (m_Chunks.contains(firstChunkToDelete)) {
            chunkProcessingQueue.emplace(firstChunkToDelete);
            chunksToDelete.emplace(firstChunkToDelete);
        }
    }

    while (!chunkProcessingQueue.empty()) {
        glm::i16vec2 chunkPos = chunkProcessingQueue.front();
        chunkProcessingQueue.pop();

        neighbours[0].x = chunkPos.x +1;                neighbours[0].y = chunkPos.y;
        neighbours[1].x = chunkPos.x -1;                neighbours[1].y = chunkPos.y;
        neighbours[2].x = chunkPos.x;                   neighbours[2].y = chunkPos.y +1;
        neighbours[3].x = chunkPos.x;                   neighbours[3].y = chunkPos.y -1;

        neighboursRelativePos[0] = neighbours[0] - currPlayerChunkPos;
        neighboursRelativePos[1] = neighbours[1] - currPlayerChunkPos;
        neighboursRelativePos[2] = neighbours[2] - currPlayerChunkPos;
        neighboursRelativePos[3] = neighbours[3] - currPlayerChunkPos;

        for (int i = 0; i < noOfNeighbours; ++i) {
            if (m_Chunks.contains(neighbours[i]) && !chunksToDelete.contains(neighbours[i])) {
                if (std::abs(neighboursRelativePos[i].x) >= renderDistance || std::abs(neighboursRelativePos[i].y) >= RenderDistance) {
                    chunkProcessingQueue.emplace(neighbours[i]);
                    chunksToDelete.emplace(neighbours[i]);
                }
                else if (!chunksToUpdate.contains(neighbours[i])) {
                    chunksToUpdate.emplace(neighbours[i]);
                }
            }
        }
    }
}

void ChunkManager::UpdateChunksAroundPlayer(glm::i16vec2 prevPlayerChunkPos, glm::i16vec2 currPlayerChunkPos) {
    std::unordered_set<glm::i16vec2> chunksToUpdate;
    std::unordered_set<glm::i16vec2> chunksToDelete;
    std::unordered_set<glm::i16vec2> chunksToGenerate;

    GetChunksToDelete(prevPlayerChunkPos, currPlayerChunkPos, chunksToDelete, chunksToUpdate);
    GetChunksToGenerate(prevPlayerChunkPos, currPlayerChunkPos, chunksToGenerate, chunksToUpdate);

    {
        // submitting for terrain generation
        {
            std::unique_lock chunkGenerationLock(m_TerrainGenerationQueueMutex);
            m_TerrainGenerationQueue.insert(chunksToGenerate.begin(), chunksToGenerate.end());
        }

        {
            std::unique_lock chunkDeletionLock(m_ChunkDeletionQueueMutex);
            m_ChunkDeletionQueue.insert(chunksToDelete.begin(), chunksToDelete.end());
        }

        // TODO: move this to a dedicated update chunk method
        // updating chunks is just creating a new mesh for now
        {
            std::unique_lock meshGenerationQueueLock(m_MeshGenerationQueueMutex);
            m_MeshGenerationQueue.insert(chunksToUpdate.begin(), chunksToUpdate.end());
        }
    }
}

SubchunkTerrainView ChunkManager::GetSubchunkTerrainView(glm::i16vec2 chunkPos, int subchunkIndex) const noexcept {
    auto itr = m_Chunks.find(chunkPos);

    if (itr == m_Chunks.end() || subchunkIndex < 0 || SubChunkCountInChunk <= subchunkIndex)
        return SubchunkTerrainView(nullptr);

    return itr->second->GetSubchunkTerrainView(subchunkIndex);
}