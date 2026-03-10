#include "FTL_Controller.hpp"
#include "Common.hpp"
#include "Logger.hpp"

FTL_Controller::FTL_Controller(): host_write_cnt(0), physical_write_cnt(0) {
  std::cout << Color::GREEN << "[FTL] Controller online. System Ready." << Color::RESET << std::endl;
}

void FTL_Controller::write (LBA lba, const std::string & data) {
  std::cout << Color::CYAN << "\n[Host Request] WRITE LBA: " << lba << " | Data: " << data << Color::RESET << std::endl;
  
  host_write_cnt++; // 計數器
  if (block_manager.isGCNeeded()) {
    std::cout << Color::YELLOW << "  [FTL] Warning: Free blocks below threshold. Triggering GC..." << Color::RESET << std::endl;
    PerformGC();
  }

  PBA oldPBA = addr_mapper.getPhysicalAddr(lba);
  /* 如果位置衝突 */
  if (oldPBA != INVALID_PBA) {
    std::cout << Color::GREEN << "  -> [FTL] Detected Overwrite. Invalidating Old PBA: " << oldPBA << Color::RESET << std::endl;
    
    // 將位置設置為 invalid
    block_manager.invalidatePage(oldPBA);

    // 更新 page 狀態
    uint32_t old_blk = oldPBA / FTLConfig::PAGES_PER_BLOCK;
    uint32_t old_page = oldPBA % FTLConfig::PAGES_PER_BLOCK;
    nand_dev.setPageStatus(old_blk, old_page, PageStatus::INVALID);
  }

  /* 分配新位置 */
  PBA newPBA = block_manager.allocateNextFreePage();
  uint32_t new_blk = newPBA / FTLConfig::PAGES_PER_BLOCK;
  uint32_t new_page = newPBA % FTLConfig::PAGES_PER_BLOCK;

  nand_dev.writePage(new_blk, new_page, data); // 物理寫入
  physical_write_cnt++; // 計數器

  /* 更新 mapping */
  addr_mapper.updateMapping(lba, newPBA);
  std::cout << Color::GREEN << "[FTL] Write Operation Success. LBA " << lba 
            << " -> PBA " << newPBA << Color::RESET << std::endl;
  return;
}

std::string FTL_Controller::read(LBA lba) {
  std::cout << Color::CYAN << "\n[Host Request] READ LBA: " << lba << Color::RESET << std::endl;
  PBA pba = addr_mapper.getPhysicalAddr(lba);
  if (pba == INVALID_PBA) {
    std::cout << Color::YELLOW << "  -> [FTL] LBA " << lba  
              << " has no data. Returning empty string." << Color::RESET << std::endl;
    return "";
  }

  uint32_t blk = pba / FTLConfig::PAGES_PER_BLOCK;
  uint32_t pg = pba % FTLConfig::PAGES_PER_BLOCK;
  
  std::string data = nand_dev.readPage(blk, pg);
  std::cout << Color::GREEN << "  -> [FTL] Data found at PBA " 
            << pba << ": " << data << Color::RESET << std::endl;
  return data;
}

void FTL_Controller::trim(LBA lba) {
  std::cout << Color::CYAN << "\n[Host Request] TRIM LBA: " << lba << Color::RESET << std::endl;
  PBA pba = addr_mapper.getPhysicalAddr(lba);
  if (pba == INVALID_PBA) {
    std::cout << Color::YELLOW << "  -> [FTL] LBA " << lba << " is already empty. No action needed." << Color::RESET << std::endl;
    return;
  }

  /* 將 pba 標記無效 */
  block_manager.invalidatePage(pba); 
  
  /* 將硬體 page 標記無效 */
  uint32_t blk = pba / FTLConfig::PAGES_PER_BLOCK;
  uint32_t page = pba % FTLConfig::PAGES_PER_BLOCK;
  nand_dev.setPageStatus(blk, page, PageStatus::INVALID); 

  /* 更新 map */
  addr_mapper.updateMapping(lba, INVALID_PBA);

  std::cout << Color::GREEN << "  -> [FTL] Trim Success. PBA " << pba << " is now marked as INVALID." << Color::RESET << std::endl;
  std::cout << Color::GREEN << "  -> [FTL] GC efficiency will improve as this page won't be moved." << Color::RESET << std::endl;
  return;
}

void FTL_Controller::PerformGC() {
  std::cout << Color::MAGENTA << "  >>>> [GC START] <<<<" << Color::RESET << std::endl;

  /* 挑選要被 GC 的 block */
  uint32_t victimID = block_manager.selectVictimBlock();
  if (victimID == 0xFFFFFFFF) {
    std::cout << Color::YELLOW << "  >>>> [GC SKIP] No suitable victim found." << Color::RESET << std::endl;
    return;
  }

  /* 搬移 valid page 到乾淨的 block */
  for (uint32_t pgID = 0; pgID < FTLConfig::PAGES_PER_BLOCK; pgID++) {
    if (nand_dev.getPageStatus(victimID, pgID) == PageStatus::VALID) {
      std::string data = nand_dev.readPage(victimID, pgID);


      /* 申請 新空間*/
      PBA newPBA = block_manager.allocateNextFreePage();
      uint32_t new_blk = newPBA / FTLConfig::PAGES_PER_BLOCK;
      uint32_t new_pg = newPBA % FTLConfig::PAGES_PER_BLOCK;

      /* 寫入新檔案 */
      nand_dev.writePage(new_blk, new_pg, data);
      physical_write_cnt++;

      std::cout << Color::MAGENTA << "  >>>> [GC] Moved Valid Page from Block " << victimID << " to PBA " << newPBA << Color::RESET << std::endl;
    }
  }

  nand_dev.eraseBlocks(victimID);
  block_manager.resetBlockMetadata(victimID);

  std::cout << Color::MAGENTA << "  >>>> [GC END] Block " << victimID << " is now clean and ready for reuse." << Color::RESET << std::endl;
}


void FTL_Controller::printStatistics() const {
  double waf = (host_write_cnt == 0) ? 0 :(double)physical_write_cnt / host_write_cnt;
  std::cout << Color::BLUE << "\n========== FTL Statistics==========" << std::endl;
  std::cout << "Host Writes: " << host_write_cnt<< std::endl;
  std::cout << "Physical Writes: " <<physical_write_cnt << std::endl;
  std::cout << "Write Amplification Factor (WAF): "<< waf << std::endl;
  std::cout <<"====================================\n" << Color::RESET <<std::endl;
}

