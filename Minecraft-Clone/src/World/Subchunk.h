

#pragma once

#include "Block.h"
#include "Quirk.h"

struct AdjacentSubchunks {
	Subchunk* Front;
	Subchunk* Back;
	Subchunk* Top;
	Subchunk* Bottom;
	Subchunk* Left;
	Subchunk* Right;
};

class Subchunk {
public:
	void CreateMesh(std::vector<Quirk::QuadVertex>& mesh, const AdjacentSubchunks& adjSubchunks);

	inline Block GetBlock(int idxX, int idxY, int idxZ) const { return m_Blocks[idxX][idxY][idxZ]; }

private:
	Block m_Blocks[16][16][16];
};
