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
  // TODO(ashish): Remove as part of no_except change
  try {
    SshInitiator(
        FLAGS_host,
        FLAGS_user,
        FLAGS_command,
        [](const char* /*buffer*/, int bytes_read, ssh::Channel& channel) {
          LOG(INFO) << "Got callback with " << bytes_read << " bytes read.";
          const int expected_version = 2;
          std::string str;
          scom::WriteMessage(expected_version, "ProtobufDetails", str);
          LOG(INFO) << "Sending message (size: " << str.size()
                    << ")with following information: ";
          unsigned int version = 0;
          std::string details;
          scom::ReadMessage(version, details, str);
          std::cout << "Sending message with version " << version
                    << " and details: " << std::endl;
          std::cout << details << std::endl;
          str.append("\n");
          LOG(INFO) << "Sending size: " << str.size();
          channel.write(str.data(), str.size());
        });
  } catch (...) {
    LOG(ERROR) << "Error executing ssh demo.";
    return -1;
  }
  return 0;
}