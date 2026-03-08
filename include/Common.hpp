#pragma once
#include <cstdint>

using LBA = uint32_t;
using PBA = uint32_t;

const PBA INVALID_PBA = 0xFFFFFFFF;

namespace FTLConfig {
  const int TOTAL_BLOCKS = 1024;   // 總 blocks 數量
  const int PAGES_PER_BLOCK = 128; // 每個 block 有的 page 數量
  const int PAGE_SIZE = 4096;      // page 大小是 4KB

  // 總 page 數量
  const uint32_t TOTAL_PHYSICAL_PAGE = TOTAL_BLOCKS * PAGES_PER_BLOCK;

  // 扣掉預留空間，實際可使用的 page 數量
  const uint32_t TOTAL_LOGICAL_PAGE = static_cast<uint32_t> (TOTAL_PHYSICAL_PAGE * 0.9);

};