//
// Created by maxmati on 5/30/17.
//

#pragma once

#include <cstdint>

struct ProcessStats {
  int64_t user_time = 0;
  int64_t system_time = 0;
  int64_t max_memory = 0;

  bool terminated_normally = false;
  int exit_code = 0; //valid only if terminated_normally

};



