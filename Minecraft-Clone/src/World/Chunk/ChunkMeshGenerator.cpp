

#include "ChunkMeshGenerator.h"
#include "Subchunk.h"
#include "World/Block/BlockDataBase.h"

static void AddFaceXZLower(Mesh& mesh, const glm::vec3 pos, const BlockData& blockData);
static void AddFaceXZUpper(Mesh& mesh, const glm::vec3 pos, const BlockData& blockData);
static void AddFaceXYFront(Mesh& mesh, const glm::vec3 pos, const BlockData& blockData);
static void AddFaceXYBack (Mesh& mesh, const glm::vec3 pos, const BlockData& blockData);
static void AddFaceYZLeft (Mesh& mesh, const glm::vec3 pos, const BlockData& blockData);
static void AddFaceYZRight(Mesh& mesh, const glm::vec3 pos, const BlockData& blockData);

void SubChunkMeshGenerator::GenerateMesh() {
	Mesh& solidMesh = m_Mesh.GetMesh();

	for (int j = 0; j < SubChunkSizeY; ++j) {
		for (int i = 0; i < SubChunkSizeX; ++i) {
			for (int k = 0; k < SubChunkSizeZ; ++k) {
				const BlockData& blockData = m_SubChunk.GetBlock(i, j, k).GetBlockData();

				if (!m_SubChunk.IsSolidBlock(i, j - 1, k))
					AddFaceXZLower(solidMesh, { i, j, k }, blockData);

				if (!m_SubChunk.IsSolidBlock(i, j + 1, k))
					AddFaceXZUpper(solidMesh, { i, j, k }, blockData);

				if (!m_SubChunk.IsSolidBlock(i, j, k + 1))
					AddFaceXYFront(solidMesh, { i, j, k }, blockData);

				if (!m_SubChunk.IsSolidBlock(i, j, k - 1))
					AddFaceXYBack(solidMesh, { i, j, k }, blockData);

				if (!m_SubChunk.IsSolidBlock(i - 1, j, k))
					AddFaceYZLeft(solidMesh, { i, j, k }, blockData);

				if (!m_SubChunk.IsSolidBlock(i + 1, j, k))
					AddFaceYZRight(solidMesh, { i, j, k }, blockData);
			}
		}
	}
}


/*
										  ||||  Right Handed System  ||||
				 Y
				 |
				 |
			 010 |________________ 110
				/|         		/|
			   / |         	   / |
			  /  |         	  /  |
			 /   |         	 /   |
			/____|__________/    |
	   011 |	 |         | 111 |
		   |	 |_________|_____|__________ X
		   |    / 000      |    / 100
		   |   /           |   /
		   |  /            |  /
		   | /             | /
	   001 |/______________|/ 101
		   /
		  /
		 /
		Z
*/

static void AddTextureCoords(Mesh& mesh, const glm::vec2& pos, const glm::vec2& size) {
	mesh.UV.emplace_back(pos.x,          pos.y         );
	mesh.UV.emplace_back(pos.x + size.x, pos.y         );
	mesh.UV.emplace_back(pos.x + size.x, pos.y + size.y);

	mesh.UV.emplace_back(pos.x,          pos.y         );
	mesh.UV.emplace_back(pos.x + size.x, pos.y + size.y);
	mesh.UV.emplace_back(pos.x,          pos.y + size.y);
}

static void AddFaceYZLeft(Mesh& mesh, const glm::vec3 pos, const BlockData& blockData) {
	mesh.Positions.emplace_back(pos.x, pos.y,        pos.z       );
	mesh.Positions.emplace_back(pos.x, pos.y,        pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x, pos.y + 1.0f, pos.z + 1.0f);

	mesh.Positions.emplace_back(pos.x, pos.y,        pos.z       );
	mesh.Positions.emplace_back(pos.x, pos.y + 1.0f, pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x, pos.y + 1.0f, pos.z       );

	for (int i = 0; i < 6; ++i)
		mesh.Normals.emplace_back(-1, 0, 0);

	// texture coords
	AddTextureCoords(mesh, blockData.SideFace.Pos, blockData.SideFace.Size);
}

static void AddFaceYZRight(Mesh& mesh, const glm::vec3 pos, const BlockData& blockData) {
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y,        pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y,        pos.z       );
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y + 1.0f, pos.z       );

	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y,        pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y + 1.0f, pos.z       );
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y + 1.0f, pos.z + 1.0f);

	for (int i = 0; i < 6; ++i)
		mesh.Normals.emplace_back(1, 0, 0);

	// texture coords
	AddTextureCoords(mesh, blockData.SideFace.Pos, blockData.SideFace.Size);
}

static void AddFaceXZLower(Mesh& mesh, const glm::vec3 pos, const BlockData& blockData) {
	mesh.Positions.emplace_back(pos.x,        pos.y, pos.z       );
	mesh.Positions.emplace_back(pos.x,        pos.y, pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y, pos.z + 1.0f);

	mesh.Positions.emplace_back(pos.x,        pos.y, pos.z       );
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y, pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y, pos.z       );

	for (int i = 0; i < 6; ++i)
		mesh.Normals.emplace_back(0, -1, 0);

	// texture coords
	AddTextureCoords(mesh, blockData.BottomFace.Pos, blockData.BottomFace.Size);
}

static void AddFaceXZUpper(Mesh& mesh, const glm::vec3 pos, const BlockData& blockData) {
	mesh.Positions.emplace_back(pos.x,        pos.y + 1.0f, pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y + 1.0f, pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y + 1.0f, pos.z       );

	mesh.Positions.emplace_back(pos.x,        pos.y + 1.0f, pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y + 1.0f, pos.z       );
	mesh.Positions.emplace_back(pos.x,        pos.y + 1.0f, pos.z       );

	for (int i = 0; i < 6; ++i)
		mesh.Normals.emplace_back(0, 1, 0);

	// texture coords
	AddTextureCoords(mesh, blockData.TopFace.Pos, blockData.TopFace.Size);
}

static void AddFaceXYFront(Mesh& mesh, const glm::vec3 pos, const BlockData& blockData) {
	mesh.Positions.emplace_back(pos.x,        pos.y,        pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y,        pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y + 1.0f, pos.z + 1.0f);

	mesh.Positions.emplace_back(pos.x,        pos.y,        pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y + 1.0f, pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x,        pos.y + 1.0f, pos.z + 1.0f);

	for (int i = 0; i < 6; ++i)
		mesh.Normals.emplace_back(0, 0, 1);

	// texture coords
	AddTextureCoords(mesh, blockData.SideFace.Pos, blockData.SideFace.Size);
}

static void AddFaceXYBack(Mesh& mesh, const glm::vec3 pos, const BlockData& blockData) {
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y,        pos.z);
	mesh.Positions.emplace_back(pos.x,        pos.y,        pos.z);
	mesh.Positions.emplace_back(pos.x,        pos.y + 1.0f, pos.z);

	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y,        pos.z);
	mesh.Positions.emplace_back(pos.x,        pos.y + 1.0f, pos.z);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y + 1.0f, pos.z);

	for (int i = 0; i < 6; ++i)
		mesh.Normals.emplace_back(0, 0, -1);

	// texture coords
	AddTextureCoords(mesh, blockData.SideFace.Pos, blockData.SideFace.Size);
}

