

#include "Quirk.h"
#include "BlockId.h"



static std::unordered_map<std::string_view, BlockId> const BlockTable = {
	{ "None",              BlockId::None        },
	{ "air",               BlockId::Air         },
	{ "dirt",              BlockId::Dirt        },
	{ "grass",             BlockId::Grass       },
	{ "cobblestone",       BlockId::CobbleStone }
};

std::string_view BlockIdToString(BlockId id) {
	switch (id) {
		case BlockId::None:                    return "None";
		case BlockId::Air:                     return "air";
		case BlockId::Dirt:                    return "dirt";
		case BlockId::Grass:                   return "grass";
		case BlockId::CobbleStone:             return "cobblestone";
	}

	QK_WARN("Invalid BlockId \"{0}\" provided", static_cast<int>(id));
	return "NONE";
}

BlockId StringToBlockId(const std::string& block) {
	if (BlockTable.contains(block)) {
		return BlockTable.at(block);
	}

	QK_WARN("Invalid BlockString \"{0}\" provided", block);
	return BlockId::None;
}
