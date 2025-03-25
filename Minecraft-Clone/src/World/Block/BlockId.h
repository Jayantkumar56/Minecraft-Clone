

#pragma once
#include <utility>
#include <string_view>

enum BlockId : uint16_t {
	None = 0,
	Air,
	Dirt,
	Grass,

	TOTAL_TYPES
};

std::string_view BlockIdToString ( BlockId id               );
BlockId          StringToBlockId ( const std::string& block );
