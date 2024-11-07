

#include "TextureManager.h"

std::vector<uint16_t> TextureManager::m_BlockTextureMapping;

// Block Textures
TextureAtlas TextureManager::m_TextureAtlas("assets/Textures/Blocks/block_textures1.png", 16, 16);

void TextureManager::LoadTextures() {
	m_BlockTextureMapping.reserve(static_cast<std::size_t>(BlockType::NoOfTypes * 6));

	SetBlockTextureMapping(BlockType::Dirt, BlockSide::Front , 0);
	SetBlockTextureMapping(BlockType::Dirt, BlockSide::Back	 , 0);
	SetBlockTextureMapping(BlockType::Dirt, BlockSide::Top	 , 0);
	SetBlockTextureMapping(BlockType::Dirt, BlockSide::Bottom, 0);
	SetBlockTextureMapping(BlockType::Dirt, BlockSide::Left	 , 0);
	SetBlockTextureMapping(BlockType::Dirt, BlockSide::Right , 0);

	SetBlockTextureMapping(BlockType::Stone, BlockSide::Front , 19);
	SetBlockTextureMapping(BlockType::Stone, BlockSide::Back  , 19);
	SetBlockTextureMapping(BlockType::Stone, BlockSide::Top	  , 19);
	SetBlockTextureMapping(BlockType::Stone, BlockSide::Bottom, 19);
	SetBlockTextureMapping(BlockType::Stone, BlockSide::Left  , 19);
	SetBlockTextureMapping(BlockType::Stone, BlockSide::Right , 19);

	SetBlockTextureMapping(BlockType::Grass, BlockSide::Front , 1);
	SetBlockTextureMapping(BlockType::Grass, BlockSide::Back  , 1);
	SetBlockTextureMapping(BlockType::Grass, BlockSide::Top   , 1);
	SetBlockTextureMapping(BlockType::Grass, BlockSide::Bottom, 1);
	SetBlockTextureMapping(BlockType::Grass, BlockSide::Left  , 1);
	SetBlockTextureMapping(BlockType::Grass, BlockSide::Right , 1);
}


