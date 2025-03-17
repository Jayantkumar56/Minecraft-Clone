

#pragma once

#include "Quirk.h"
#include "SpriteSheet.h"

class TextureManager {
public:
	TextureManager() {
		LoadSpriteSheets();
	}

private:
	void LoadSpriteSheets() {
		m_SpriteSheets.emplace_back("", "");
	}

private:
	std::vector<SpriteSheet> m_SpriteSheets;
};
