

#pragma once

#include "World/Block.h"
#include "Quirk.h"

class TextureAtlas {
public:
	TextureAtlas(const std::string& filePath, uint16_t tileWidth, uint16_t tileHeight) : 
			m_Texture(Quirk::Texture2D::Create(filePath)),
			m_TileWidth(tileWidth),
			m_TileHeight(tileHeight)
	{
	}

	inline Quirk::SubTexture2D GetTexture(uint16_t index) {
		uint16_t tilesInX = m_Texture->GetWidth() / m_TileWidth;

		uint16_t indexX = index % tilesInX;
		uint16_t indexY = index / tilesInX;

		return Quirk::SubTexture2D(m_Texture, indexX * m_TileWidth, indexY * m_TileHeight, m_TileWidth, m_TileHeight);
	}

private:
	uint16_t m_TileWidth, m_TileHeight;
	Quirk::Ref<Quirk::Texture2D> m_Texture;
};

class TextureManager {
public:
	static void LoadTextures();

	static inline Quirk::SubTexture2D GetTexture(BlockType blockType, BlockSide side) {
		int16_t index = m_BlockTextureMapping[static_cast<size_t>((blockType * 6) + side)];
		return m_TextureAtlas.GetTexture(index);
	}

private:
	static inline void SetBlockTextureMapping(BlockType blockType, BlockSide side, uint16_t index) {
		m_BlockTextureMapping[static_cast<size_t>((blockType * 6) + side)] = index;
	}

private:
	static std::vector<uint16_t> m_BlockTextureMapping;
	static TextureAtlas m_TextureAtlas;
};
