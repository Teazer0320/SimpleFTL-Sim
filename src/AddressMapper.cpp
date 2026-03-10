#include "AddressMapper.hpp"
#include "Logger.hpp"
#include <iostream>

AddressMapper::AddressMapper() {
  mapping_table.assign(FTLConfig::TOTAL_LOGICAL_PAGE, INVALID_PBA);
  std::cout << Color::GREEN << "[Mapper] Initialized L2P Table with " 
              << FTLConfig::TOTAL_LOGICAL_PAGE << " entries." << Color::RESET << std::endl;
}

void AddressMapper::updateMapping(LBA lba, PBA pba) {
  if (lba >= FTLConfig::TOTAL_LOGICAL_PAGE) {
    std::cerr << Color::RED << "  [Mapper Error] LBA " << lba 
              << " is out of logical range!" << Color::RESET << std::endl;
    return;
  }

  // 記錄舊的 pba
  PBA old_pba = mapping_table[lba];
  // 更新 pba
  mapping_table[lba] = pba;

  if (old_pba == INVALID_PBA) {
    std::cout << Color::GREEN << "  -> [Mapper] First write for LBA " << lba 
              << ". Mapping to PBA " << pba << Color::RESET << std::endl;
  } else {
    std::cout << Color::GREEN << "  -> [Mapper] Update Mapping: LBA " << lba 
              << " | Old PBA: " << old_pba << " -> New PBA  "
              << pba << Color::RESET << std::endl;
  }
  return;
}

PBA AddressMapper::getPhysicalAddr(LBA lba) const {
  if (lba >= FTLConfig::TOTAL_LOGICAL_PAGE) {
    return INVALID_PBA;
  }
  return mapping_table[lba];
}

bool AddressMapper::isValidMapped(LBA lba) const {
  return getPhysicalAddr(lba) != INVALID_PBA;
}