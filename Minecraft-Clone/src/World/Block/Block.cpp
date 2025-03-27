

#include "Block.h"
#include "BlockDataBase.h"



const BlockData& ChunkBlock::GetBlockData() const {
	return BlockDataBase::GetBlockData(Id);
}
