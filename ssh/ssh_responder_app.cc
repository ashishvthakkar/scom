#include <config.h>

#include "../expressions/utils.h"
#include "ssh_responder.h"

int main() {
  scom::SshResponder ssh_responder(kResponderLog);
  while (true) {
    std::string in = ssh_responder.Receive();
    if (in.empty()) {
      break;
    }
    int result = code_experiments::Compute(in);
    std::string response = std::to_string(result);
    ssh_responder.Send(response);
  }
  return 0;
}