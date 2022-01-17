#include <config.h>
#include <expressions/utils.h>

#include "ssh_responder.h"

int main() {
  scom::SshResponder ssh_responder(kResponderLog);
  while (true) {
    std::string in = ssh_responder.Receive();
    if (in.empty()) {
      break;
    }
    // Echo back the request
    ssh_responder.Send(in);
  }
  return 0;
}