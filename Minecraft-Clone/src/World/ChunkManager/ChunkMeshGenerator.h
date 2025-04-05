

#pragma once


#include "SubchunkView.h"

class SubChunkMeshGenerator {
public:
	SubChunkMeshGenerator(ChunkMesh& mesh, std::vector<ChunkBlock>& blocks)
	{}

	static void Generate(ChunkMesh& mesh, SubchunkTerrainView terrain, glm::i16vec3 pos);

private:
	static void AddTextureCoords(Mesh& mesh, const glm::vec2& pos, const glm::vec2& size);

	static void AddFaceXZLower(Mesh& mesh, const glm::vec3& pos, const BlockData& blockData);
	static void AddFaceXZUpper(Mesh& mesh, const glm::vec3& pos, const BlockData& blockData);
	static void AddFaceXYFront(Mesh& mesh, const glm::vec3& pos, const BlockData& blockData);
	static void AddFaceXYBack (Mesh& mesh, const glm::vec3& pos, const BlockData& blockData);
	static void AddFaceYZLeft (Mesh& mesh, const glm::vec3& pos, const BlockData& blockData);
	static void AddFaceYZRight(Mesh& mesh, const glm::vec3& pos, const BlockData& blockData);
};

