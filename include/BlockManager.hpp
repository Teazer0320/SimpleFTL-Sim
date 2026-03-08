#pragma once
#include <vector>
#include <queue>
#include <set>

enum class BlockStatus { FREE, DATA, DIRTY, BAD};

struct Block {
  int id;
  int eraseCount = 0;
  int invalidPageCount = 0;
  BlockStatus status = BlockStatus::FREE;
};


class BlockManager {
public:
  BlockManager(int totalBlocks);
  int getFreeBlock();               // get 可寫入的 block ID

  void invalidatePage(int blockId); // 當 page 無效時計數
  int selectVictimBlock();          // 用 Greedy 挑選回價值最高的 block ID;
  void eraseBlco(int blockID);      // 清除 block 內部資料，並歸還 free list 內

private:
  std::queue <Block> freeList_;     // 存放可使用的 block 
  std::vector<Block> blocks_;       // all blocks;
  
  /* TODO 
    完成 Wear leveling 的優先權隊列
  */

};

