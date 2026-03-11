#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "FTL_Controller.hpp"
#include "Logger.hpp"

class TracePlayer {
public:
  TracePlayer(FTL_Controller& ftl) : ftl_(ftl) {};
  void play(const std::string& trace_file);
private:
  FTL_Controller ftl_;
};
