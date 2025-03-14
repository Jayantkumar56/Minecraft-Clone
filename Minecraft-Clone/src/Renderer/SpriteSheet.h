

#pragma once

#include "Quirk.h"

struct SpriteData {
	uint16_t PosX  = 0, PosY   = 0;
	uint16_t width = 0, height = 0;
};

class SpriteSheet {
public:
	SpriteSheet(Quirk::Ref<Quirk::Texture2D> texture, std::unordered_map<std::string, SpriteData> spriteRegistry) :
			m_Texture        ( std::move(texture)        ),
			m_SpriteRegistry ( std::move(spriteRegistry) )
	{}

	inline SpriteData GetSprteData(const std::string& spriteName) {
		SpriteData data;
		if (m_SpriteRegistry.contains(spriteName)) {
			data = m_SpriteRegistry[spriteName];
		}
		return data;
	}

private:
	Quirk::Ref<Quirk::Texture2D> m_Texture;
	std::unordered_map<std::string, SpriteData> m_SpriteRegistry;
};
