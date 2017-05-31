//
// Created by maxmati on 5/30/17.
//

#include "Runner.h"

#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <cassert>
#include <algorithm>

static double timeEvalToDouble(const timeval &t) {
  return t.tv_sec + t.tv_usec * 1.e-6d;
}

ProcessStats Runner::Run(std::vector<std::string> argv) {

  pid_t pid = fork();
  if (0 == pid) {
    ChildEntry(std::move(argv));
  } else if (-1 == pid) {
    abort();
  }

  ProcessStats stats = ProcessStats();

  WaitForPid(pid, stats);

  rusage usage;
  getrusage(RUSAGE_CHILDREN, &usage);

  stats.user_time = timeEvalToDouble(usage.ru_utime);
  stats.system_time = timeEvalToDouble(usage.ru_stime);
  stats.max_memory = usage.ru_maxrss;

  return stats;
}

void Runner::WaitForPid(pid_t pid, ProcessStats &stats) {
  int status;

  pid_t waited_pid = waitpid(pid, &status, 0);
  if (waited_pid != pid) {
    abort();
  }

  if (WIFEXITED(status)) {
    stats.terminated_normally = true;
    stats.exit_code = WEXITSTATUS(status);
  }

}
void Runner::ChildEntry(std::vector<std::string> argv) {
  assert(argv.size() >= 1);

  std::vector<const char *> unpackedArgv(argv.size());
  std::transform(argv.begin(), argv.end(), unpackedArgv.begin(), [](auto s) { return s.c_str(); });

  //TODO: prevent starting child processes

  if (STDERR_FILENO != dup2(STDOUT_FILENO, STDERR_FILENO)) {
    abort();
  }

  execve(unpackedArgv[0], const_cast<char **>(unpackedArgv.data()), {nullptr});
}
