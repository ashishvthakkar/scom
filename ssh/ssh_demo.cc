#include <config.h>
#include <gflags/gflags.h>

#include "../protocol/protocol_adapter.h"
#include "ssh_initiator.h"

DEFINE_string(host, "", "[localhost]");  // NOLINT
DEFINE_string(user, "", "[username]");   // NOLINT
DEFINE_string(                           // NOLINT
    command,
    "",
    "[/home/ashish/git/scom/build/ssh/ssh_responder -request=ping]");

int main(int argc, char** argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  std::array<std::string, 2> requests{"Request1", "Request2"};
  // TODO(ashish): Remove as part of no_except change
  try {
    auto i = 0;
    SshInitiator(
        FLAGS_host,
        FLAGS_user,
        FLAGS_command,
        [&requests,
         &i](const char* buffer, int bytes_read, ssh::Channel& channel) {
          if (bytes_read > 0) {
            LOG(INFO) << "Got callback with " << bytes_read << " bytes read.";
            // TODO(ashish): Update to scom::read
            LOG(INFO) << "Got response: " << buffer;
          }
          if (i >= requests.size()) {
            return true;  // done
          }
          LOG(INFO) << "Sending request: " << requests.at(i);
          std::string protobuf_request;
          scom::WriteMessage(
              kProtocolVersion,
              requests.at(i),
              protobuf_request);
          protobuf_request.append("\n");
          channel.write(protobuf_request.data(), protobuf_request.size());
          i++;
          return false;  // not done yet
        });
  } catch (...) {
    LOG(ERROR) << "Error executing ssh demo.";
    return -1;
  }
  return 0;
}