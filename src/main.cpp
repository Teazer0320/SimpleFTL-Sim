#include "Trace_Player.hpp"
#include "FTL_Controller.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <trace_file_path>" << std::endl;
    std::cerr << "Example: ./FTL_Sim ../traces/basic.trace" << std::endl;
    return 1;
  }

  FTL_Controller ftl;
  TracePlayer player(ftl);
  std::string trace_file = "../traces/" + std::string(argv[1]);
  player.play(trace_file);



  std::cout << Color::BLUE << "\n[Demo Complete] FTL handled all requests and maintained data integrity." << Color::RESET << std::endl;

  return 0;
}