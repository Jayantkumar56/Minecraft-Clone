

#include "SpriteSheet.h"

static uint16_t FlipUVVertically(uint16_t v, uint16_t height, uint16_t spriteSheetHeight);

void SpriteSheet::DeserializeSpriteRegistry(const std::filesystem::path& spriteRegistryPath) {
	YAML::Node data;
	try {
		data = YAML::LoadFile(spriteRegistryPath.string());
	}
	catch (YAML::ParserException e) {
		QK_CORE_ERROR("Failed to load project file");
		return;
	}

	auto spriteSheetNode = data["SpriteSheet"];
	if (!spriteSheetNode)
		return;

	SpriteData spriteData;

	uint16_t spriteWidth  = spriteSheetNode[ "Width"  ].as<uint16_t>();
	uint16_t spriteHeight = spriteSheetNode[ "Height" ].as<uint16_t>();

	QK_ASSERT(
		spriteWidth == m_Texture->GetWidth() && spriteHeight == m_Texture->GetHeight(),
		"Texture size and size in SpriteRegistry are different!"
	);

	auto sprites = spriteSheetNode["Sprites"];

	for (auto sprite : sprites) {
		spriteData.Name   = sprite["Name"].as<std::string>();
		spriteData.PosX   = sprite[ "U"     ].as<uint16_t>();
		spriteData.PosY   = sprite[ "V"     ].as<uint16_t>();
		spriteData.Width  = sprite[ "Width" ].as<uint16_t>();
		spriteData.Height = sprite[ "Height"].as<uint16_t>();

		spriteData.PosY = FlipUVVertically(spriteData.PosY, spriteData.Height, spriteHeight);

		m_SpriteRegistry.emplace_back(spriteData);
	}
}

static uint16_t FlipUVVertically(uint16_t v, uint16_t height, uint16_t spriteSheetHeight) {
	return spriteSheetHeight - (v + height);
}

