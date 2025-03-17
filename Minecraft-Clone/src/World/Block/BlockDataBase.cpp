


#include "BlockDataBase.h"


static std::string GetBlockNameFromSpriteName  (const std::string& spriteName);
static std::string GetBlockSidesFromSpriteName (const std::string& spriteName);



void BlockDataBase::MapSpritesData(const SpriteSheet& spriteSheet) {
	auto& spriteRegistry = spriteSheet.GetSpriteRegistry();

	for (const auto& spriteData : spriteRegistry) {
		const auto blockName  = GetBlockNameFromSpriteName(spriteData.Name);
		const auto blockSides = GetBlockSidesFromSpriteName(spriteData.Name);

		BlockId id = StringToBlockId(blockName);
		m_BlockRegistry[id].SpriteSheetTexture = spriteSheet.GetTexture();

		if (blockSides.find("TOP") != std::string::npos) {
			m_BlockRegistry[id].TopFace.Pos.x  = spriteData.PosX;
			m_BlockRegistry[id].TopFace.Pos.y  = spriteData.PosY;
			m_BlockRegistry[id].TopFace.Size.x = spriteData.Width;
			m_BlockRegistry[id].TopFace.Size.y = spriteData.Height;
		}
		if (blockSides.find("SIDE") != std::string::npos) {
			m_BlockRegistry[id].SideFace.Pos.x  = spriteData.PosX;
			m_BlockRegistry[id].SideFace.Pos.y  = spriteData.PosY;
			m_BlockRegistry[id].SideFace.Size.x = spriteData.Width;
			m_BlockRegistry[id].SideFace.Size.y = spriteData.Height;
		}
		if (blockSides.find("BOTTOM") != std::string::npos) {
			m_BlockRegistry[id].BottomFace.Pos.x  = spriteData.PosX;
			m_BlockRegistry[id].BottomFace.Pos.y  = spriteData.PosY;
			m_BlockRegistry[id].BottomFace.Size.x = spriteData.Width;
			m_BlockRegistry[id].BottomFace.Size.y = spriteData.Height;
		}
	}
}


static std::string GetBlockNameFromSpriteName(const std::string& spriteName) {
	size_t seperatorIndex = spriteName.find("__");
	return std::string(spriteName.begin(), spriteName.begin() + seperatorIndex);
}

static std::string GetBlockSidesFromSpriteName(const std::string& spriteName) {
	size_t seperatorIndex = spriteName.find("__");
	return std::string(spriteName.begin() + seperatorIndex, spriteName.end());
}
