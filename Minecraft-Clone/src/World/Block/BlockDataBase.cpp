


#include "BlockDataBase.h"

BlockDataBase* BlockDataBase::s_Instance = nullptr;

static std::string GetBlockNameFromSpriteName  (const std::string& spriteName);
static std::string GetBlockSidesFromSpriteName (const std::string& spriteName);


void BlockDataBase::MapSpritesData(const SpriteSheet& spriteSheet) {
	auto& spriteRegistry  = spriteSheet.GetSpriteRegistry();
	auto  spriteSheetSize = spriteSheet.GetSize();

	glm::vec2 spritePos  (0.0f);
	glm::vec2 spriteSize (0.0f);

	for (const auto& spriteData : spriteRegistry) {
		const auto blockName  = GetBlockNameFromSpriteName(spriteData.Name);
		const auto blockSides = GetBlockSidesFromSpriteName(spriteData.Name);

		BlockId id = StringToBlockId(blockName);
		m_BlockRegistry[id].SpriteSheetTexture = spriteSheet.GetTexture();

		spritePos.x = static_cast<float>(spriteData.PosX) / spriteSheetSize.x;
		spritePos.y = static_cast<float>(spriteData.PosY) / spriteSheetSize.y;

		spriteSize.x = static_cast<float>(spriteData.Width)  / spriteSheetSize.x;
		spriteSize.y = static_cast<float>(spriteData.Height) / spriteSheetSize.y;

		if (blockSides.find("TOP") != std::string::npos) {
			m_BlockRegistry[id].TopFace.Pos  = spritePos;
			m_BlockRegistry[id].TopFace.Size = spriteSize;
		}
		if (blockSides.find("SIDE") != std::string::npos) {
			m_BlockRegistry[id].SideFace.Pos  = spritePos;
			m_BlockRegistry[id].SideFace.Size = spriteSize;
		}
		if (blockSides.find("BOTTOM") != std::string::npos) {
			m_BlockRegistry[id].BottomFace.Pos  = spritePos;
			m_BlockRegistry[id].BottomFace.Size = spriteSize;
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
