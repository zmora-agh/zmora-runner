#include <iostream>
#include <algorithm>
#include "Runner.h"

static std::vector<std::string> parsArgs(int argc, char **argv) {
  std::vector<std::string> args(argc - 1);
  std::transform(argv + 1, argv + argc, args.begin(), [](auto s) { return s; });
  return args;
}

int main(int argc, char **argv) {
  Runner r;
  r.Run(std::move(parsArgs(argc, argv)));
  return 0;
}