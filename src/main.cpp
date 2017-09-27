#include "Runner.h"

#include <iostream>
#include <algorithm>
#include <iterator>
#include "json.hpp"

using json = nlohmann::json;

json toJson(const ProcessStats &stats) {
  json j;
  j["exit_code"] = stats.exit_code;
  j["terminated_normally"] = stats.terminated_normally;
  j["max_memory"] = stats.max_memory;
  j["system_time"] = stats.system_time;
  j["user_time"] = stats.user_time;
  j["timeouted"] = stats.timeouted;
  return j;
}

static std::vector<std::string> convertArgs(int argc, char **argv) {
  std::vector<std::string> args(argc - 1);
  std::transform(argv + 1, argv + argc, args.begin(), [](auto s) { return s; });
  return args;
}

static bool handleShowVersion(const Config& config) {
  if(config.showVersion)
    std::cout << "Zmora-runner version " << VERSION << std::endl;
  return config.showVersion;
}

static Config parseArgs(std::vector<std::string>&& vector) {
  Config config{};

  auto it = begin(vector);
  for(; it != end(vector); ++it){
    if(it->find("--") != 0) break;

    if(*it == "--timeout") {
      try {
        if(++it == end(vector)){
          std::cerr<<"Error: you have to specify timeout"<<std::endl;
          exit(-1);
        }
        const auto timeout = stoul(*it);
        if(timeout > std::numeric_limits<uint>::max()){
          std::cerr<<"Error: timeout is out of range"<<std::endl;
          exit(-1);
        }
        config.timeout = static_cast<uint>(timeout);
      } catch (const std::invalid_argument& e){
        std::cerr<<"Error: timeout have to be numeric"<<std::endl;
        exit(-1);
      } catch (const std::out_of_range& e){
        std::cerr<<"Error: timeout is out of range"<<std::endl;
        exit(-1);
      }
    } else if (*it == "--version") {
      config.showVersion = true;
    }
  }

  std::move(it, end(vector), back_inserter(config.childArgv));

  return config;
}

static bool validateConfig(const Config& config){
  if(config.showVersion) return true;
  if(config.childArgv.empty()) {
    std::cerr<<"Error: missing program to run"<<std::endl;
    return false;
  }
  return true;
}

int main(int argc, char **argv) {
  Runner r;
  auto args = convertArgs(argc, argv);
  Config config = parseArgs(std::move(args));
  if(!validateConfig(config)) return -1;
  if(handleShowVersion(config)) return 0;

  ProcessStats stats = r.Run(config);
  std::cerr << toJson(stats) << std::flush;
  return 0;
}