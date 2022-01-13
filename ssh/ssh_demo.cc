#include "ssh_initiator.h"

int main() {
  // TODO(ashish): Remove as part of no_except change
  try {
    SshInitiator(
        "ash-alienw-r11",
        "ashish",
        "/home/ashish/git/scom/build/ssh/ssh_responder -request=ping",
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