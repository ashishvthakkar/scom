#include <gflags/gflags.h>

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
          std::string addition_input("additional_input_from_callback\n");
          channel.write(addition_input.data(), addition_input.size());
        });
  } catch (...) {
    LOG(ERROR) << "Error executing ssh demo.";
    return -1;
  }
  return 0;
}