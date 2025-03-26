

#pragma once

#include "Renderer/Mesh.h"


class ChunkMesh {
public:
	Mesh& GetMesh() { return m_Mesh; }

private:
	Mesh m_Mesh;
};
