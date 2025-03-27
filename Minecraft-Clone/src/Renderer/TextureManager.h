

#pragma once

#include "Quirk.h"
#include "SpriteSheet.h"
#include "World/Block/BlockDataBase.h"

class TextureManager {
public:
	void LoadBlockSprites(BlockDataBase& database) {
		const auto& spriteSheet1 = m_SpriteSheets.emplace_back(
			"assets/SpriteSheets/spritesheet1.png", 
			"assets/SpriteSheets/spritesheet1.yaml"
		);

		database.MapSpritesData(spriteSheet1);
	}

	// temporary 
	auto GetSpriteSheetTexture() { return m_SpriteSheets[0].GetTexture(); }

private:
	std::vector<SpriteSheet> m_SpriteSheets;
};
