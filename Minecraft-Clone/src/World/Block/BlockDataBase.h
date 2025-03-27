

#pragma once

#include "Block.h"
#include "Renderer/SpriteSheet.h"

#include <array>


class BlockDataBase {
public:
	BlockDataBase() { s_Instance = this; }

	void MapSpritesData(const SpriteSheet& spriteSheet);

	static const auto& GetBlockData(BlockId id) { return s_Instance->m_BlockRegistry[id]; }

private:
	// index of the array refers to the BlockId
	std::array<BlockData, static_cast<size_t>(BlockId::TOTAL_TYPES)> m_BlockRegistry;

	static BlockDataBase* s_Instance;
};

