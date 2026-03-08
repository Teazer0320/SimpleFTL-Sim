#pragma once
#include "AddressMapper.hpp"
#include "BlockManager.hpp"
#include "NandDevice.hpp"

class FTL_Controller {
public:
  FTL_Controller();

  /* 主要功能 */
  void write(LBA lba, const std::string & data);
  std::string read(LBA lba);
  void trim(LBA lba);   // Host 刪除請求

  // 其他
  void printStatistics() const;

private:
  /* 將 address mapper、block manager、 Nand device 實例化*/
  AddressMapper addr_mapper;
  BlockManager block_manager;
  NandDevice nand_dev;

  /* 內部的計數器 */
  uint64_t host_write_cnt;
  uint64_t physical_write_cnt;


  /* Garbage Collection */
  void PerformGC();
  
  // log 輸出
  void logStep(const std::string& module, const std::string& message) const;

};