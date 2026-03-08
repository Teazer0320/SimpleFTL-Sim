#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>

namespace Config {
  const uint32_t PAGE_SIZE = 4096;     // page size: 4 KB
  const uint32_t PAGE_PER_BLOCK = 128; // 每個 block 有 128 個 page
}

enum class PageStatus {
  CLEAN,      // 可直接寫入
  VALID,      // 已存放合法資料
  INVALID,    // 資料過時
};

struct Page {
  PageStatus status = PageStatus::CLEAN;
  std::string data = ""; // 模擬用來儲存資料
};

class NandDevice {
public:
  NandDevice();

  // 基礎操作定義
  void writePage(uint32_t blockID, uint32_t pageID, const std::string & data ); // 寫入
  std::string readPage(uint32_t blockID, uint32_t pageID) const; // 讀取（不能修改）
  void eraseBlocks(uint32_t blockID); // 清除 block; 

  // 輔助
  PageStatus getPageStatus(uint32_t blockID, uint32_t pageID) const;
  int getTotalBlocks() const {return totalBlocks_};


private:
  int totalBlocks_; // 儲存這個 NandDevice 總共有多少 blocks

  // 二維資料結構模擬 blocks & page
  std::vector<std::vector<Page>> storage_; // strage_[0] -> block 0; storage_[0][2] -> block 0 中的 page [2];

  // 位置檢查函式
  bool isValidAddress(uint32_t blockID, uint32_t pageID) const;

}