

#pragma once

#include "utility"

enum class BlockType : uint16_t {
	None, Air, Grass
};


struct Block {
	BlockType Type;
};
