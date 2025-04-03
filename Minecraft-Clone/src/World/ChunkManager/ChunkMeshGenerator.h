

#pragma once

#include "World/Chunk/Constants.h"
#include "World/Chunk/ChunkMesh.h"
#include "World/Block/Block.h"

class SubChunk;
class Chunk;

class SubChunkMeshGenerator {
public:
	SubChunkMeshGenerator(SubChunk& subchunk, ChunkMesh& mesh, std::vector<ChunkBlock>& blocks) :
			m_SubChunk ( subchunk ),
			m_Mesh     ( mesh     ), 
			m_Blocks   ( blocks   )
	{}

	void GenerateMesh();

private:
	SubChunk&  m_SubChunk;
	ChunkMesh& m_Mesh;
	std::vector<ChunkBlock>& m_Blocks;
};


class ChunkMeshGenerator {
public:
	static void Generate(Chunk& chunk);
};
