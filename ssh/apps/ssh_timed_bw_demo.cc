#include <config.h>
#include <gflags/gflags.h>
#include <glog/logging.h>

#include <chrono>

#include "../ssh_initiator.h"

DEFINE_string(host, "", "[localhost]");  // NOLINT
DEFINE_string(user, "", "[username]");   // NOLINT
DEFINE_string(                           // NOLINT
    command,
    "",
    "[/home/ashish/git/scom/build/ssh/ssh_echo_responder]");
DEFINE_int64(num_requests, 10000, "[number of requests]");   // NOLINT
DEFINE_int64(request_size, 4000, "[size of each request]");  // NOLINT

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  const auto request_size = FLAGS_request_size;
  const auto num_requests = FLAGS_num_requests;
  std::string message(request_size, 'a');
  LOG(INFO) << "Sending each request with size: " << message.size();
  scom::SshInitiator initiator(
      FLAGS_host,
      FLAGS_user,
      FLAGS_command,
      kUsePublicKeyAuth);
  namespace chrono = std::chrono;
  auto start = chrono::high_resolution_clock::now();
  for (auto i = 0; i < num_requests; i++) {
    initiator.Send(message);
    initiator.Receive(message);
    // Disable logging during perf run
    // LOG(INFO) << "Got response with size: " << response.size();
    // LOG(INFO) << "Got response: " << response;
  }

  auto end = chrono::high_resolution_clock::now();

  const auto to_mega = 1024 * 1024;
  auto seconds = chrono::duration_cast<chrono::seconds>(end - start).count();
  auto size = (request_size * num_requests) / to_mega;
  LOG(INFO) << "Completing total requests with size " << size
            << " MB and received responses with the same total size in "
            << seconds << " seconds for a bandwidth of " << size / seconds
            << "MB/s.";

  return 0;
}