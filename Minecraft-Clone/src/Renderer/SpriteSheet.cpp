

#include "SpriteSheet.h"


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

	auto sprites = data["Sprites"];
	for (auto sprite : sprites) {
		spriteData.Name   = sprite["Name"].as<std::string>();
		spriteData.PosX   = sprite[ "U"     ].as<uint16_t>();
		spriteData.PosY   = sprite[ "V"     ].as<uint16_t>();
		spriteData.Width  = sprite[ "Width" ].as<uint16_t>();
		spriteData.Height = sprite[ "Height"].as<uint16_t>();

		m_SpriteRegistry.emplace_back(spriteData);
	}
}
