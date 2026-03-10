#pragma once
#include <string>

namespace Color {
  const std::string RESET   = "\033[0m";
  const std::string RED     = "\033[31m";   // Error
  const std::string GREEN   = "\033[32m";   // Normal operation
  const std::string YELLOW  = "\033[33m";   // Warning
  const std::string BLUE    = "\033[34m";   // Statistics
  const std::string MAGENTA = "\033[35m";   // Garbage Collection
  const std::string CYAN    = "\033[36m";   // Host Request
}
