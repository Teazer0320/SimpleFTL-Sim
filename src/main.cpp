#include "FTL_Controller.hpp"
#include "Logger.hpp"
#include <iostream>
#include <string>
#include <vector>

int main(void) {
  FTL_Controller ftl; // 宣告一個 FTL instance
  
  /* 模擬 write */
  ftl.write(10, "write 1");
  ftl.write(20, "write 2");

  /* 模擬 read */
  std::cout << "\n--- Step 1: Basic Write/Read Test ---" << std::endl;
  std::cout << "read addr 10 :" << ftl.read(10) << std::endl;
  std::cout << "read addr 20 :" << ftl.read(20) << std::endl;
  std::cout << "read addr 30 :" << ftl.read(30) << std::endl;


  /* 模擬 寫入至重複位置 */
  std::cout << "\n--- Step 2: Overwrite Test (Check Invalidating) ---" << std::endl;
  ftl.write(10, "update data at 10");
  std::cout << "read addr 10 :" << ftl.read(10)  << std::endl;
  
  /* 模擬 大量寫入 */
  std::cout << "\n--- Step 3: Heavy Write to Trigger GC ---" << std::endl;
  for (uint32_t i = 100; i < 600; i++) {
    ftl.write(i, "write "+ std::to_string(i));
    if (i % 50 == 0) {
      std::cout << "successfully write " + std::to_string(i)  << std::endl;
    }
  }

  /* 模擬 trim */
  std::cout << "\n--- Step 4: Trim Test ---" << std::endl;
  ftl.write(50, "Trash data");
  ftl.trim(50);
  std::cout << "read addr 50 : " << ftl.read(50)  << std::endl;


  /* 輸出報告 */
  std::cout << "\n--- Step 5: Final Performance Report ---" << std::endl;
  ftl.printStatistics();
  std::cout << Color::BLUE << "\n[Demo Complete] FTL handled all requests and maintained data integrity." << Color::RESET << std::endl;

  return 0;
}