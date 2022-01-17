#include <config.h>
#include <gflags/gflags.h>
#include <glog/logging.h>

#include <chrono>

#include "ssh_requestor.h"

DEFINE_string(host, "", "[localhost]");  // NOLINT
DEFINE_string(user, "", "[username]");   // NOLINT
DEFINE_string(                           // NOLINT
    command,
    "",
    "[/home/ashish/git/scom/build/ssh/ssh_responder]");

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  std::vector<std::string> requests{"5 + 2", "3*7"};

  const auto num_requests = 10'000;
  const auto num_repeats = num_requests / requests.size();

  scom::SshRequestor requestor(FLAGS_host, FLAGS_user, FLAGS_command);
  namespace chrono = std::chrono;
  auto start = chrono::high_resolution_clock::now();
  for (int i = 0; i < num_repeats; i++) {
    for (const auto& request : requests) {
      LOG(INFO) << "Sending request: " << request;
      const auto& response = requestor.SendReceive(request);
      LOG(INFO) << "Got response: " << response;
    }
  }
  auto end = chrono::high_resolution_clock::now();
  LOG(INFO) << "Completing " << (num_repeats * requests.size())
            << " requests took "
            << chrono::duration_cast<chrono::seconds>(end - start).count()
            << " seconds.";

  return 0;
}