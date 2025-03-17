

#pragma once

#include "Quirk.h"

struct SpriteData {
	std::string Name;
	uint16_t    PosX  = 0,   PosY   = 0;
	uint16_t    Width = 0,   Height = 0;
};

class SpriteSheet {
public:
	SpriteSheet(const std::filesystem::path& texturePath, const std::filesystem::path& spriteRegistryPath) {
		m_Texture = Quirk::Texture2D::Create(texturePath);
		DeserializeSpriteRegistry(spriteRegistryPath);
	}

	inline void ReleaseSpriteRegistry() { m_SpriteRegistry.clear(); }
	inline auto GetTexture() const { return m_Texture; }

	inline const auto& GetSpriteRegistry() const noexcept { return m_SpriteRegistry; }
	inline glm::vec2   GetSize()           const { return glm::vec2(m_Texture->GetWidth(), m_Texture->GetHeight()); }

private:
	void DeserializeSpriteRegistry (const std::filesystem::path& spriteRegistryPath);

private:
	Quirk::Ref<Quirk::Texture2D> m_Texture;
	std::vector<SpriteData> m_SpriteRegistry;
};
