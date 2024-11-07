

#pragma once

#include <utility>

enum BlockType : uint16_t {
	Air = 0,
	Dirt, Stone, Grass, 
	NoOfTypes
};

enum BlockSide {
	Top		= 0, 
	Bottom	= 1, 
	Left	= 2, 
	Right	= 3, 
	Front	= 4, 
	Back	= 5
};

class Block {
public:
	BlockType GetType() const { return m_Type; }

private:
	BlockType m_Type = BlockType::Air;
};
