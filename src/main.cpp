#include <iostream>
#include <algorithm>
#include "Runner.h"
#include "json.hpp"

using json = nlohmann::json;

json toJson(const ProcessStats &stats) {
  json j;
  j["exit_code"] = stats.exit_code;
  j["terminated_normally"] = stats.terminated_normally;
  j["max_memory"] = stats.max_memory;
  j["system_time"] = stats.system_time;
  j["user_time"] = stats.user_time;
  return j;
}

static std::vector<std::string> parsArgs(int argc, char **argv) {
  std::vector<std::string> args(argc - 1);
  std::transform(argv + 1, argv + argc, args.begin(), [](auto s) { return s; });
  return args;
}

static bool handleShowVersion(std::vector<std::string> args) {
  if(args.size() == 0 || args.front() != "--version" ) return false;
  std::cout << "Zmora-runner version " << VERSION << std::endl;
  return true;
}

int main(int argc, char **argv) {
  Runner r;
  auto args = parsArgs(argc, argv);
  if(handleShowVersion(args)) return 0;
  
  ProcessStats stats = r.Run(std::move(args));
  std::cerr << toJson(stats) << std::flush;
  return 0;
}