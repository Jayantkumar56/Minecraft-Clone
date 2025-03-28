

#pragma once

#include "Quirk.h"

#include "BlockId.h"
#include "glm/glm.hpp"



enum class BlockFace : uint16_t {
	Top = 0,
	Sides,
	Bottom,

	TOTAL_TYPES
};

struct BlockSpriteData {
	glm::vec2 Pos;
	glm::vec2 Size;

};

struct BlockData {
	// sprite data for top face, side faces and bottom face of the block
	BlockSpriteData TopFace;
	BlockSpriteData SideFace;
	BlockSpriteData BottomFace;

	Quirk::Ref<Quirk::Texture2D> SpriteSheetTexture;
};

struct ChunkBlock {
	const BlockData& GetBlockData() const;

	BlockId Id = BlockId::Air;
};
