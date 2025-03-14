

#pragma once

#include "Quirk.h"
#include "SpriteSheet.h"

class TextureManager {
public:


private:
	void LoadSpriteSheets();

private:
	std::vector<SpriteSheet> m_SpriteSheets;
};
