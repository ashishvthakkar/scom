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
    "[/home/ashish/git/scom/build/ssh/ssh_echo_responder]");
DEFINE_int32(num_requests, 10000, "[number of requests]");   // NOLINT
DEFINE_int32(request_size, 4000, "[size of each request]");  // NOLINT

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  const auto request_size = FLAGS_request_size;
  const auto num_requests = FLAGS_num_requests;
  std::string request(request_size, 'a');
  LOG(INFO) << "Sending each request with size: " << request.size();
  scom::SshRequestor requestor(FLAGS_host, FLAGS_user, FLAGS_command);
  namespace chrono = std::chrono;
  auto start = chrono::high_resolution_clock::now();
  for (auto i = 0; i < num_requests; i++) {
    const auto& response = requestor.SendReceive(request);
    // Disable logging during perf run
    // LOG(INFO) << "Got response with size: " << response.size();
    // LOG(INFO) << "Got response: " << response;
  }

  auto end = chrono::high_resolution_clock::now();
  LOG(INFO) << "Completing total requests with size "
            << (request_size * num_requests)
            << " and receive responses with the same total size "
            << chrono::duration_cast<chrono::seconds>(end - start).count()
            << " seconds.";

  return 0;
}