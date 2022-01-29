#include <config.h>
#include <gflags/gflags.h>
#include <glog/logging.h>

#include "../ssh_requestor.h"

DEFINE_string(host, "", "[localhost]");  // NOLINT
DEFINE_string(user, "", "[username]");   // NOLINT
DEFINE_string(                           // NOLINT
    command,
    "",
    "[/home/ashish/git/scom/build/ssh/ssh_responder]");

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  std::vector<std::string> requests{"5 + 2", "3*7"};

  scom::SshRequestor requestor(FLAGS_host, FLAGS_user, FLAGS_command);
  int request_id = 0;
  int for_request_id = 0;
  for (const auto& request : requests) {
    LOG(INFO) << "Sending request: " << request << " with request id "
              << request_id;
    requestor.Send(request, request_id);
    request_id++;
    const auto& response = requestor.Receive(for_request_id);
    LOG(INFO) << "Got response: " << response << " for request id "
              << for_request_id;
  }
  return 0;
}