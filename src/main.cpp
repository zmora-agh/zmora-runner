#include <iostream>
#include "Runner.h"

int main() {
  Runner r;
  r.Run({"/bin/ls", "-l"});
    std::cout << "Hello, World!" << std::endl;
    return 0;
}