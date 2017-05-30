//
// Created by maxmati on 5/30/17.
//

#pragma once

struct ProcessStats {
  double user_time = 0;
  double system_time = 0;
  long max_memory = 0;

  bool terminated_normally = false;
  int exit_code = 0; //valid only if terminated_normally

};



