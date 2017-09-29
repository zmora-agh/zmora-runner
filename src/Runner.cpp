//
// Created by maxmati on 5/30/17.
//

#include "Runner.h"

#include <unistd.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <cassert>
#include <algorithm>
#include <iostream>

pid_t childPid = 0;
bool timeouted = false;

static void handleTimeout(int){

  timeouted = true;
  kill(childPid, SIGKILL);
}

static int64_t timeEvalToMicros(const timeval &t) {
  return t.tv_sec * 1000000ll + t.tv_usec;
}

ProcessStats Runner::Run(const Config& config) {

  signal(SIGALRM, handleTimeout);

  pid_t pid = fork();
  if (0 == pid) {
    ChildEntry(config.childArgv);
  } else if (-1 == pid) {
    abort();
  }

  childPid = pid;
  alarm(config.timeout);

  ProcessStats stats{};

  WaitForPid(pid, stats);

  rusage usage;
  getrusage(RUSAGE_CHILDREN, &usage);

  stats.user_time = timeEvalToMicros(usage.ru_utime);
  stats.system_time = timeEvalToMicros(usage.ru_stime);
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
  stats.timeouted = timeouted;

}
void Runner::ChildEntry(const std::vector<std::string>& argv) {
  assert(argv.size() >= 1);

  std::vector<const char *> unpackedArgv(argv.size() + 1);
  std::transform(argv.begin(), argv.end(), unpackedArgv.begin(), [](const auto& s) { return s.c_str(); });
  unpackedArgv.push_back(nullptr);

  //TODO: prevent starting child processes

  if (STDERR_FILENO != dup2(STDOUT_FILENO, STDERR_FILENO)) {
    abort();
  }

  char* const envp[] = {nullptr};

  execve(unpackedArgv[0], const_cast<char **>(unpackedArgv.data()), envp);
}
