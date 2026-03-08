#include "Common.hpp"
#include "NandDevice.hpp"
#include <stdexcept>
#include <iostream>

// constructor
NandDevice::NandDevice () {
  std::cout << "[NAND] Initialized " << FTLConfig::TOTAL_BLOCKS 
            << " blocks, each with " << FTLConfig::PAGES_PER_BLOCK 
            << " pages." << std::endl;
}


void NandDevice::writePage(uint32_t blockID, uint32_t pageID, const std::string & data) {
  // Invalid Physical address
  if (!isValidAddress(blockID, pageID)) {
    throw std::out_of_range("[NAND Error] Invalid Physical Address!");
  }

  Page & targetPage = storage_[blockID][pageID];

  // 如果當前 page 不是 clean 不能寫入
  if (targetPage.status != PageStatus::CLEAN) {
    std::cerr << "[NAND Error] Program Fail! Block " << blockID 
              << " Page " << pageID << " is not clean (Current Status: " 
              << (targetPage.status == PageStatus::VALID ? "VALID" : "INVALID") << ")" << std::endl;
    throw std::runtime_error("NAND Program Failure: Page already written.");
  }

  // 執行 write
  targetPage.data = data;
  targetPage.status = PageStatus::VALID;
  std::cout << "  >> [NAND] Physical Write: Block " << blockID 
            << ", Page " << pageID << " | Data: " << data << std::endl;
  
}

std::string NandDevice::readPage (uint32_t blockID, uint32_t pageID) const {
  if (!isValidAddress(blockID, pageID)) {
    return "";
  }  

  const Page & targetPage = storage_[blockID][pageID];
  
  // 若非有效 page
  if (targetPage.status != PageStatus::VALID) {
    std::cout << "  >> [NAND] Warning: Reading from non-valid page (" << blockID << "," << pageID << ")" << std::endl;
  }

  return targetPage.data;
}


void NandDevice::eraseBlocks(uint32_t blockID) {
  if (blockID >= FTLConfig::TOTAL_BLOCKS) {
    return;
  }

  std::cout << "[NAND] Erasing Block " << blockID << " ..." << std::endl;

  // 逐一初始化
  for (uint32_t pageIdx = 0; pageIdx < FTLConfig::PAGES_PER_BLOCK; pageIdx++) {
    storage_[blockID][pageIdx].status = PageStatus::CLEAN;
    storage_[blockID][pageIdx].data = "";
  }
  std::cout << "  >> [NAND] Block " << blockID << " is now CLEAN." << std::endl;
}

PageStatus NandDevice::getPageStatus(uint32_t blockID, uint32_t pageID) const {
  if (!isValidAddress(blockID, pageID)) return PageStatus::INVALID;
  return storage_[blockID][pageID].status;
}

bool NandDevice::isValidAddress(uint32_t blockID, uint32_t pageID) const {
  if (blockID < FTLConfig::TOTAL_BLOCKS && pageID < FTLConfig::PAGES_PER_BLOCK) {
      return true;
    }
  return false;
}