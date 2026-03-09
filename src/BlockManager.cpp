#include <iostream>
#include "BlockManager.hpp"
#include "Common.hpp"

BlockManager::BlockManager(): current_block_id_(-1), current_page_offset_(0) {
  blocks_.resize(FTLConfig::TOTAL_BLOCKS);
  for (uint32_t id = 0; id < FTLConfig::TOTAL_BLOCKS; id++) {
    blocks_[id].id = id;
    free_block_pool_.push(id);
  }
  std::cout << "[BlockMgr] Initialized with " << FTLConfig::TOTAL_BLOCKS << " free blocks." << std::endl;
}

PBA BlockManager::allocateNextFreePage() {
  if (current_block_id_ == -1 || current_page_offset_ > FTLConfig::PAGES_PER_BLOCK) {
    current_block_id_ = fetchFreeBlock();
    current_page_offset_ = 0;

    std::cout << "    >> [BlockMgr] Switched to Block " << current_block_id_ << std::endl;
  }

  PBA pba = (current_block_id_ * FTLConfig::PAGES_PER_BLOCK) + current_page_offset_; 
  current_page_offset_++; // 更新 page offset 指針

  return pba;
}

void BlockManager::invalidatePage(PBA pba) {
  uint32_t block_id = pba / FTLConfig::PAGES_PER_BLOCK;
  if (block_id < FTLConfig::TOTAL_BLOCKS) {
    blocks_[block_id].invalidPageCount++;
  }
  return;
}

bool BlockManager::isGCNeeded () const {
  return (free_block_pool_.size() < FTLConfig::GC_THRESHOLD_BLOCKS);
}


uint32_t BlockManager::selectVictimBlock() {
  uint32_t victimID = 0xFFFFFFF;
  uint32_t maxInvalid = 0;

  for (uint32_t i = 0; i < FTLConfig::TOTAL_BLOCKS; i++) {
    if (i == current_block_id_) continue; // 當前正在寫入的 block 不可回收
    if (blocks_[i].invalidPageCount > maxInvalid) {
      maxInvalid = blocks_[i].invalidPageCount;
      victimID = i;
    }
  }
  return victimID;
}

void BlockManager::resetBlockMetadata(uint32_t blockID) {
  if (blockID > FTLConfig::TOTAL_BLOCKS) return;
  blocks_[blockID].eraseCount = 0;
  blocks_[blockID].invalidPageCount = 0;
  blocks_[blockID].freePageOffset = 0;

  free_block_pool_.push(blockID);
}

uint32_t BlockManager::fetchFreeBlock() {
  if (free_block_pool_.empty()) {
    throw std::runtime_error("No free blocks available! GC should have been triggered.");
  }
  uint32_t id = free_block_pool_.front();
  free_block_pool_.pop();
  return id;
}
