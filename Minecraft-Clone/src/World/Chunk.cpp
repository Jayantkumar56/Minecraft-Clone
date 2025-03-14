

#include "Chunk.h"



static void AddFaceXZLower(Mesh& mesh, const glm::vec3 pos);
static void AddFaceXZUpper(Mesh& mesh, const glm::vec3 pos);
static void AddFaceXYFront(Mesh& mesh, const glm::vec3 pos);
static void AddFaceXYBack (Mesh& mesh, const glm::vec3 pos);
static void AddFaceYZLeft (Mesh& mesh, const glm::vec3 pos);
static void AddFaceYZRight(Mesh& mesh, const glm::vec3 pos);


void SubChunk::GenerateMesh() {
	for (int j = 0; j < SubChunkSizeY; ++j) {
		for (int i = 0; i < SubChunkSizeX; ++i) {
			for (int k = 0; k < SubChunkSizeZ; ++k) {
				if (!IsSolidBlock(i, j - 1, k))
					AddFaceXZLower(m_Mesh, { i, j, k });

				if (!IsSolidBlock(i, j + 1, k))
					AddFaceXZUpper(m_Mesh, { i, j + 1, k });

				if (!IsSolidBlock(i, j, k + 1))
					AddFaceXYFront(m_Mesh, { i, j, k + 1 });

				if (!IsSolidBlock(i, j, k - 1))
					AddFaceXYBack(m_Mesh, { i, j, k });

				if (!IsSolidBlock(i - 1, j, k))
					AddFaceYZLeft(m_Mesh, { i, j, k });

				if (!IsSolidBlock(i + 1, j, k))
					AddFaceYZRight(m_Mesh, { i + 1, j, k });
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

static void AddFaceYZLeft(Mesh& mesh, const glm::vec3 pos) {
	mesh.Positions.emplace_back(pos.x, pos.y   , pos.z   );
	mesh.Positions.emplace_back(pos.x, pos.y   , pos.z +1);
	mesh.Positions.emplace_back(pos.x, pos.y +1, pos.z +1);

	mesh.Positions.emplace_back(pos.x, pos.y   , pos.z   );
	mesh.Positions.emplace_back(pos.x, pos.y +1, pos.z +1);
	mesh.Positions.emplace_back(pos.x, pos.y +1, pos.z   );

	for (int i = 0; i < 6; ++i)
		mesh.Normals.emplace_back(-1, 0, 0);


}

static void AddFaceYZRight(Mesh& mesh, const glm::vec3 pos) {
	mesh.Positions.emplace_back(pos.x, pos.y,     pos.z    );
	mesh.Positions.emplace_back(pos.x, pos.y + 1, pos.z + 1);
	mesh.Positions.emplace_back(pos.x, pos.y,     pos.z + 1);

	mesh.Positions.emplace_back(pos.x, pos.y,     pos.z    );
	mesh.Positions.emplace_back(pos.x, pos.y + 1, pos.z    );
	mesh.Positions.emplace_back(pos.x, pos.y + 1, pos.z + 1);

	for (int i = 0; i < 6; ++i)
		mesh.Normals.emplace_back(1, 0, 0);
}

static void AddFaceXZLower(Mesh& mesh, const glm::vec3 pos) {
	mesh.Positions.emplace_back(pos.x,    pos.y, pos.z   );
	mesh.Positions.emplace_back(pos.x,    pos.y, pos.z +1);
	mesh.Positions.emplace_back(pos.x +1, pos.y, pos.z +1);

	mesh.Positions.emplace_back(pos.x,    pos.y, pos.z   );
	mesh.Positions.emplace_back(pos.x +1, pos.y, pos.z +1);
	mesh.Positions.emplace_back(pos.x +1, pos.y, pos.z   );

	for (int i = 0; i < 6; ++i)
		mesh.Normals.emplace_back(0, -1, 0);
}

static void AddFaceXZUpper(Mesh& mesh, const glm::vec3 pos) {
	mesh.Positions.emplace_back(pos.x,    pos.y, pos.z   );
	mesh.Positions.emplace_back(pos.x +1, pos.y, pos.z +1);
	mesh.Positions.emplace_back(pos.x,    pos.y, pos.z +1);

	mesh.Positions.emplace_back(pos.x,    pos.y, pos.z   );
	mesh.Positions.emplace_back(pos.x +1, pos.y, pos.z   );
	mesh.Positions.emplace_back(pos.x +1, pos.y, pos.z +1);

	for (int i = 0; i < 6; ++i)
		mesh.Normals.emplace_back(0, 1, 0);
}

static void AddFaceXYFront(Mesh& mesh, const glm::vec3 pos) {
	mesh.Positions.emplace_back(pos.x,    pos.y   , pos.z);
	mesh.Positions.emplace_back(pos.x,    pos.y +1, pos.z);
	mesh.Positions.emplace_back(pos.x +1, pos.y +1, pos.z);

	mesh.Positions.emplace_back(pos.x,    pos.y   , pos.z);
	mesh.Positions.emplace_back(pos.x +1, pos.y +1, pos.z);
	mesh.Positions.emplace_back(pos.x +1, pos.y   , pos.z);

	for (int i = 0; i < 6; ++i)
		mesh.Normals.emplace_back(0, 0, 1);
}

static void AddFaceXYBack(Mesh& mesh, const glm::vec3 pos) {
	mesh.Positions.emplace_back(pos.x,    pos.y   , pos.z);
	mesh.Positions.emplace_back(pos.x +1, pos.y +1, pos.z);
	mesh.Positions.emplace_back(pos.x,    pos.y +1, pos.z);

	mesh.Positions.emplace_back(pos.x,    pos.y   , pos.z);
	mesh.Positions.emplace_back(pos.x +1, pos.y   , pos.z);
	mesh.Positions.emplace_back(pos.x +1, pos.y +1, pos.z);

	for (int i = 0; i < 6; ++i)
		mesh.Normals.emplace_back(0, 0, -1);
}
