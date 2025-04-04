

#include "ChunkMeshGenerator.h"


void SubChunkMeshGenerator::Generate(ChunkMesh& mesh, SubchunkTerrainView terrain, glm::i16vec3 pos) {
	Mesh& solidMesh   = mesh.GetMesh();
	auto  subchunkPos = pos;

	for (int j = 0; j < SubChunkSizeY; ++j) {
		for (int i = 0; i < SubChunkSizeX; ++i) {
			for (int k = 0; k < SubChunkSizeZ; ++k) {
				const BlockData& blockData = terrain.GetBlock(i, j, k).GetBlockData();

				glm::vec3 blockPos(
					static_cast<float>(subchunkPos.x) + i,
					static_cast<float>(subchunkPos.y) + j,
					static_cast<float>(subchunkPos.z) + k
				);

				if (!terrain.IsSolidBlock(i, j - 1, k))
					AddFaceXZLower(solidMesh, blockPos, blockData);

				if (!terrain.IsSolidBlock(i, j + 1, k))
					AddFaceXZUpper(solidMesh, blockPos, blockData);

				if (!terrain.IsSolidBlock(i, j, k + 1))
					AddFaceXYFront(solidMesh, blockPos, blockData);

				if (!terrain.IsSolidBlock(i, j, k - 1))
					AddFaceXYBack(solidMesh, blockPos, blockData);

				if (!terrain.IsSolidBlock(i - 1, j, k))
					AddFaceYZLeft(solidMesh, blockPos, blockData);

				if (!terrain.IsSolidBlock(i + 1, j, k))
					AddFaceYZRight(solidMesh, blockPos, blockData);
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

void SubChunkMeshGenerator::AddTextureCoords(Mesh& mesh, const glm::vec2& pos, const glm::vec2& size) {
	mesh.UV.emplace_back(pos.x, pos.y);
	mesh.UV.emplace_back(pos.x + size.x, pos.y);
	mesh.UV.emplace_back(pos.x + size.x, pos.y + size.y);

	mesh.UV.emplace_back(pos.x, pos.y);
	mesh.UV.emplace_back(pos.x + size.x, pos.y + size.y);
	mesh.UV.emplace_back(pos.x, pos.y + size.y);
}

void SubChunkMeshGenerator::AddFaceYZLeft(Mesh& mesh, const glm::vec3& pos, const BlockData& blockData) {
	mesh.Positions.emplace_back(pos.x, pos.y, pos.z);
	mesh.Positions.emplace_back(pos.x, pos.y, pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x, pos.y + 1.0f, pos.z + 1.0f);

	mesh.Positions.emplace_back(pos.x, pos.y, pos.z);
	mesh.Positions.emplace_back(pos.x, pos.y + 1.0f, pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x, pos.y + 1.0f, pos.z);

	for (int i = 0; i < 6; ++i)
		mesh.Normals.emplace_back(-1, 0, 0);

	// texture coords
	AddTextureCoords(mesh, blockData.SideFace.Pos, blockData.SideFace.Size);
}

void SubChunkMeshGenerator::AddFaceYZRight(Mesh& mesh, const glm::vec3& pos, const BlockData& blockData) {
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y, pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y, pos.z);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y + 1.0f, pos.z);

	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y, pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y + 1.0f, pos.z);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y + 1.0f, pos.z + 1.0f);

	for (int i = 0; i < 6; ++i)
		mesh.Normals.emplace_back(1, 0, 0);

	// texture coords
	AddTextureCoords(mesh, blockData.SideFace.Pos, blockData.SideFace.Size);
}

void SubChunkMeshGenerator::AddFaceXZLower(Mesh& mesh, const glm::vec3& pos, const BlockData& blockData) {
	mesh.Positions.emplace_back(pos.x, pos.y, pos.z);
	mesh.Positions.emplace_back(pos.x, pos.y, pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y, pos.z + 1.0f);

	mesh.Positions.emplace_back(pos.x, pos.y, pos.z);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y, pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y, pos.z);

	for (int i = 0; i < 6; ++i)
		mesh.Normals.emplace_back(0, -1, 0);

	// texture coords
	AddTextureCoords(mesh, blockData.BottomFace.Pos, blockData.BottomFace.Size);
}

void SubChunkMeshGenerator::AddFaceXZUpper(Mesh& mesh, const glm::vec3& pos, const BlockData& blockData) {
	mesh.Positions.emplace_back(pos.x, pos.y + 1.0f, pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y + 1.0f, pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y + 1.0f, pos.z);

	mesh.Positions.emplace_back(pos.x, pos.y + 1.0f, pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y + 1.0f, pos.z);
	mesh.Positions.emplace_back(pos.x, pos.y + 1.0f, pos.z);

	for (int i = 0; i < 6; ++i)
		mesh.Normals.emplace_back(0, 1, 0);

	// texture coords
	AddTextureCoords(mesh, blockData.TopFace.Pos, blockData.TopFace.Size);
}

void SubChunkMeshGenerator::AddFaceXYFront(Mesh& mesh, const glm::vec3& pos, const BlockData& blockData) {
	mesh.Positions.emplace_back(pos.x, pos.y, pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y, pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y + 1.0f, pos.z + 1.0f);

	mesh.Positions.emplace_back(pos.x, pos.y, pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x + 1.0f, pos.y + 1.0f, pos.z + 1.0f);
	mesh.Positions.emplace_back(pos.x, pos.y + 1.0f, pos.z + 1.0f);

	for (int i = 0; i < 6; ++i)
		mesh.Normals.emplace_back(0, 0, 1);

	// texture coords
	AddTextureCoords(mesh, blockData.SideFace.Pos, blockData.SideFace.Size);
}

void SubChunkMeshGenerator::AddFaceXYBack(Mesh& mesh, const glm::vec3& pos, const BlockData& blockData) {
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
