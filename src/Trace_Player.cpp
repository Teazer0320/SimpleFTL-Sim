#include "Trace_Player.hpp"

void TracePlayer::play(const std::string& trace_file) {
  // read trace file
  std::ifstream file(trace_file);
  if (!file.is_open()) {
    std::cerr << "Failed to open trace file: " << trace_file << std::endl;
    return;
  }

  std::string line;

  while (std::getline(file, line)) {
    // skip empty lines and comments
    if (line.empty() || line[0] == '#') continue;

    char op;
    uint32_t lba, size;

    std::istringstream ss(line);
    if (!(ss >> op >> lba >> size)) continue; // skip malformed lines

    switch(op) {
      case 'R':
        std::cout << Color::BLUE << "[TracePlayer] Read Request: LBA " << lba 
                  << " | Size: " << size << Color::RESET << std::endl;
        ftl_.read(lba);
        break;

      case 'W':
        std::cout<<Color::BLUE << "[TracePlayer] Write Request: LBA " << lba 
                 << " | Size: " << size << Color::RESET << std::endl;
        ftl_.write(lba, "data"); // 模擬寫入資料
        break;

      case 'T':
        std::cout << Color::BLUE << "[TracePlayer] Trim Request: LBA " << lba 
                  << " | Size: " << size << Color::RESET << std::endl;
        ftl_.trim(lba);
        break;
      default:
        std::cerr << "Unknown operation: " << op <<   std::endl;
        break;
    }

    std::cout << std::endl; // 每筆請求後換行
  }

  std::cout << Color::GREEN << "[TracePlayer] Output Performance Report " << std::endl;
  ftl_.printStatistics();

  std::cout<< Color::GREEN << "\n[TracePlayer] Finished processing trace file: " << trace_file << Color::RESET << std::endl;

}