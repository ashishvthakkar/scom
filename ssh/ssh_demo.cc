#include <config.h>
#include <gflags/gflags.h>

#include "../protocol/protocol_adapter.h"
#include "ssh_requestor.h"

DEFINE_string(host, "", "[localhost]");  // NOLINT
DEFINE_string(user, "", "[username]");   // NOLINT
DEFINE_string(                           // NOLINT
    command,
    "",
    "[/home/ashish/git/scom/build/ssh/ssh_responder]");

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  std::vector<std::string> requests{"Request1", "Request2"};
  SshRequestor requestor(FLAGS_host, FLAGS_user, FLAGS_command);
  for (const auto& request : requests) {
    const auto& response = requestor.Send(request);
    LOG(INFO) << "Got response with size: " << response.size();
  }
  return 0;
}