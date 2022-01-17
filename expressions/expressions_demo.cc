#include <gflags/gflags.h>
#include <glog/logging.h>

#include <chrono>

#include "utils.h"

DEFINE_string(command, "", "Specify -command=compute or simplify"); // NOLINT
DEFINE_string(expression, "", "Specify expression"); // NOLINT

void Compute(const std::string &expression) {
  namespace chrono = std::chrono;
  auto start = chrono::high_resolution_clock::now();
  auto result = code_experiments::Compute(expression);
  auto end = chrono::high_resolution_clock::now();
  LOG(INFO) << expression << " = " << result;
  LOG(INFO) << "This computation took "
            << chrono::duration_cast<chrono::nanoseconds>(end - start).count()
            << " nanoseconds.";
}

void Simplify(const std::string &expression) {
  namespace chrono = std::chrono;
  auto start = chrono::high_resolution_clock::now();
  auto result = code_experiments::Simplify(expression);
  auto end = chrono::high_resolution_clock::now();
  LOG(INFO) << expression << " = " << result;
  LOG(INFO) << "This simplifcation took "
            << chrono::duration_cast<chrono::nanoseconds>(end - start).count()
            << " nanoseconds.";
}

int main(int argc, char **argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  LOG(INFO) << "Expressions demo.";
  if (FLAGS_command == "compute") {
    Compute(FLAGS_expression);
  } else if (FLAGS_command == "simplify") {
    Simplify(FLAGS_expression);
  } else {
    LOG(ERROR) << "Expected -command=compute or -command=simplify";
  }

  LOG(INFO) << "Done.";
  return 0;
}