

#pragma once


#include "SubchunkView.h"

class SubChunkMeshGenerator {
public:
	SubChunkMeshGenerator(glm::i16vec3 pos,ChunkMesh& mesh, SubchunkTerrainViewEx terrain) :
			m_Pos     ( pos     ),
			m_Mesh    ( mesh    ),
			m_Terrain ( terrain )
	{
	}

	void Generate();

private:
	void AddTextureCoords(Mesh& mesh, const glm::vec2& pos, const glm::vec2& size);

	void AddFaceXZLower(Mesh& mesh, const glm::vec3& pos, const BlockData& blockData);
	void AddFaceXZUpper(Mesh& mesh, const glm::vec3& pos, const BlockData& blockData);
	void AddFaceXYFront(Mesh& mesh, const glm::vec3& pos, const BlockData& blockData);
	void AddFaceXYBack (Mesh& mesh, const glm::vec3& pos, const BlockData& blockData);
	void AddFaceYZLeft (Mesh& mesh, const glm::vec3& pos, const BlockData& blockData);
	void AddFaceYZRight(Mesh& mesh, const glm::vec3& pos, const BlockData& blockData);

private:
	glm::i16vec3 m_Pos;
	ChunkMesh&   m_Mesh;
	SubchunkTerrainViewEx m_Terrain;
};

