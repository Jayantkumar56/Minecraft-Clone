

#pragma once

#include "Constants.h"
#include "ChunkMesh.h"
#include "World/Block/Block.h"

class SubChunk;

class SubChunkMeshGenerator {
public:
	SubChunkMeshGenerator(SubChunk& subchunk, ChunkMesh& mesh, std::array<ChunkBlock, SubChunkBlocksCount>& blocks) :
			m_SubChunk ( subchunk ),
			m_Mesh     ( mesh     ), 
			m_Blocks   ( blocks   )
	{}

	void GenerateMesh();

private:
	SubChunk&  m_SubChunk;
	ChunkMesh& m_Mesh;
	std::array<ChunkBlock, SubChunkBlocksCount>& m_Blocks;
};
