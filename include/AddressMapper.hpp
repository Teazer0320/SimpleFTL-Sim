#pragma once
#include <vector>
#include "Common.hpp"

class AddressMapper {
public:
  AddressMapper();
  void updateMapping(LBA lba, PBA pba); // 更新邏輯位置映射的真實位置
  PBA getPhysicalAddr(LBA lba) const;  // 取得 lab 映射的 pba
  bool isValidMapped(LBA lba) const;    // 確認是否為有效映射

private:
  std::vector <PBA> mapping_table;
};
