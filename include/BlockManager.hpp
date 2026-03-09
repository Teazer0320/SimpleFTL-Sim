#pragma once
#include <vector>
#include <queue>
#include "Common.hpp"

// 區塊元數據
struct BlockInfo {
  uint32_t id;
  uint32_t eraseCount = 0;
  uint32_t invalidPageCount = 0;
  uint32_t freePageOffset = 0; // 該 Block 內下一個可寫入的 Page Offset
};

class BlockManager {
public:
  BlockManager();
  
  
  PBA allocateNextFreePage();   // 取得下一個可寫入的物理位址 (PBA)

  void invalidatePage(PBA pba); // 當 LBA 更新時，告知 BlockManager 舊的 PBA 已失效
  
  bool isGCNeeded() const;      // 檢查是否需要啟動垃圾回收 (GC)
  
  uint32_t selectVictimBlock(); // 挑選回收價值最高的 Block (Greedy Policy)
  
  void resetBlockMetadata(uint32_t blockID);  // 重置區塊狀態 (用於 GC 擦除後)

private:
    uint32_t fetchFreeBlock(); // 從 Pool 中拿一個乾淨的 Block

    std::vector<BlockInfo> blocks_;
    std::queue<uint32_t> free_block_pool_;
    
    int current_block_id_;     // 當前正在寫入的 Block
    uint32_t current_page_offset_; // 當前寫到第幾頁
};