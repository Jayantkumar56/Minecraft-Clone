

#pragma once

#include "Block.h"
#include "Renderer/SpriteSheet.h"

#include <array>


class BlockDataBase {
public:
	void MapSpritesData(const SpriteSheet& spriteSheet);

	const auto& GetBlockData(BlockId id) const { return m_BlockRegistry[id]; }

private:
	// index of the array refers to the BlockId
	std::array<BlockData, static_cast<size_t>(BlockId::TOTAL_TYPES)> m_BlockRegistry;
};

