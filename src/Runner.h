//
// Created by maxmati on 5/30/17.
//

#pragma once

#include "ProcessStats.h"

#include <sys/types.h>
#include <string>
#include <vector>

class Runner {
 public:
  ProcessStats Run(std::vector<std::string> argv);
 private:
  void WaitForPid(pid_t pid, ProcessStats &stats);
  void ChildEntry(std::vector<std::string> argvs);
};



