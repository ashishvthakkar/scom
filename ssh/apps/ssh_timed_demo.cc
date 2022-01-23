#include <config.h>
#include <gflags/gflags.h>
#include <glog/logging.h>

#include <chrono>

#include "../ssh_requestor.h"

DEFINE_string(host, "", "[localhost]");  // NOLINT
DEFINE_string(user, "", "[username]");   // NOLINT
DEFINE_string(                           // NOLINT
    command,
    "",
    "[/home/ashish/git/scom/build/ssh/ssh_responder]");
DEFINE_int32(num_requests, 10, "[number of requests]");  // NOLINT

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  std::vector<std::string> requests{"5 + 2", "3*7"};

  const auto num_requests = FLAGS_num_requests;
  const auto num_repeats = num_requests / requests.size();

  scom::SshRequestor requestor(FLAGS_host, FLAGS_user, FLAGS_command);
  namespace chrono = std::chrono;
  auto start = chrono::high_resolution_clock::now();
  for (auto i = 0; i < num_repeats; i++) {
    for (const auto& request : requests) {
      const auto& response = requestor.SendReceive(request);
      // Disable logging during perf run
      // LOG(INFO) << request << " -> " << response;
    }
  }
  auto end = chrono::high_resolution_clock::now();
  LOG(INFO) << "Completing " << (num_repeats * requests.size())
            << " requests took "
            << chrono::duration_cast<chrono::seconds>(end - start).count()
            << " seconds.";

  return 0;
}