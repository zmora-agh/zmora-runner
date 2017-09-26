//
// Created by maxmati on 5/30/17.
//

#pragma once

#include "ProcessStats.h"

#include <sys/types.h>
#include <string>
#include <vector>

struct Config {
  bool showVersion = false;
  uint timeout = 0;
  std::vector<std::string> childArgv = {};
};

class Runner {
 public:
  ProcessStats Run(const Config& config);
 private:
  void WaitForPid(pid_t pid, ProcessStats &stats);
  void ChildEntry(const std::vector<std::string>& argv);
};



