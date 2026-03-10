#include "Common.hpp"
#include "NandDevice.hpp"
#include "Logger.hpp"
#include <stdexcept>
#include <iostream>

// constructor
NandDevice::NandDevice()
{
  storage_.resize(FTLConfig::TOTAL_BLOCKS, std::vector<Page>(FTLConfig::PAGES_PER_BLOCK));

  std::cout << Color::GREEN << "[NAND] Initialized " << FTLConfig::TOTAL_BLOCKS
            << " blocks, each with " << FTLConfig::PAGES_PER_BLOCK
            << " pages." << Color::RESET << std::endl;
}

void NandDevice::writePage(uint32_t blockID, uint32_t pageID, const std::string &data)
{
  // Invalid Physical address
  if (!isValidAddress(blockID, pageID))
  {
    throw std::out_of_range(Color::RED + "[NAND Error] Invalid Physical Address! at blockID = " + std::to_string(blockID) + " and pageID = " + std::to_string(pageID) + "." + Color::RESET);
  }

  Page &targetPage = storage_[blockID][pageID];

  // 如果當前 page 不是 clean 不能寫入
  if (targetPage.status != PageStatus::CLEAN)
  {
    std::cerr << Color::RED << "[NAND Error] Program Fail! Block " << blockID
              << " Page " << pageID << " is not clean (Current Status: "
              << (targetPage.status == PageStatus::VALID ? "VALID" : "INVALID") << ")" << Color::RESET << std::endl;
    throw std::runtime_error("NAND Program Failure: Page already written.");
  }

  // 執行 write
  targetPage.data = data;
  targetPage.status = PageStatus::VALID;
  std::cout << Color::GREEN << "  >> [NAND] Physical Write: Block " << blockID
            << ", Page " << pageID << " | Data: " << data << Color::RESET << std::endl;
}

std::string NandDevice::readPage(uint32_t blockID, uint32_t pageID) const
{
  if (!isValidAddress(blockID, pageID))
  {
    return "";
  }

  const Page &targetPage = storage_[blockID][pageID];

  // 若非有效 page
  if (targetPage.status != PageStatus::VALID)
  {
    std::cout << Color::YELLOW << "  >> [NAND] Warning: Reading from non-valid page (" << blockID << "," << pageID << ")" << Color::RESET << std::endl;
  }

  return targetPage.data;
}

void NandDevice::eraseBlocks(uint32_t blockID)
{
  if (blockID >= FTLConfig::TOTAL_BLOCKS)
  {
    return;
  }

  std::cout << Color::GREEN << "[NAND] Erasing Block " << blockID << " ..." << Color::RESET << std::endl;

  // 逐一初始化
  for (uint32_t pageIdx = 0; pageIdx < FTLConfig::PAGES_PER_BLOCK; pageIdx++)
  {
    storage_[blockID][pageIdx].status = PageStatus::CLEAN;
    storage_[blockID][pageIdx].data = "";
  }
  std::cout << Color::GREEN << "  >> [NAND] Block " << blockID << " is now CLEAN." << Color::RESET << std::endl;
}

PageStatus NandDevice::getPageStatus(uint32_t blockID, uint32_t pageID) const
{
  if (!isValidAddress(blockID, pageID))
    return PageStatus::INVALID;
  return storage_[blockID][pageID].status;
}

void NandDevice::setPageStatus(uint32_t blockID, uint32_t pageID, PageStatus state)
{
  if (!isValidAddress(blockID, pageID))
  {
    std::cerr << Color::RED << "[NAND Error] setPageStatus: Invalid Address ("
              << blockID << "," << pageID << ")" << Color::RESET << std::endl;
    return;
  }
  Page target = storage_[blockID][pageID];

  // 確認是不是合法操作
  if (target.status == PageStatus::INVALID && state == PageStatus::VALID)
  {
    std::cerr << Color::YELLOW << "[NAND Warning] Attempting to set INVALID page to VALID \
    without Erase! Block: "
              << blockID << " Page: " << pageID << Color::RESET << std::endl;
  }

  target.status = state;

  std::cout << Color::GREEN << "    >> [NAND] Status Change: Block " << blockID
            << " Page " << pageID << " is now "
            << (state == PageStatus::VALID ? "VALID" : (state == PageStatus::INVALID ? "INVALID" : "CLEAN"))
            << Color::RESET << std::endl;

  return;
}

bool NandDevice::isValidAddress(uint32_t blockID, uint32_t pageID) const
{
  if (blockID < FTLConfig::TOTAL_BLOCKS && pageID < FTLConfig::PAGES_PER_BLOCK)
  {
    return true;
  }
  return false;
}