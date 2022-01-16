#include "ssh_responder.h"

int main() {
  SshResponder ssh_responder(kResponderLog);
  std::string input;
  // while (std::cin >> input) {
  input.resize(13);  // NOLINT TODO(ashish): change
  while (ssh_responder.ReadNextMessage(input) > 0) {
    ssh_responder.ProcessInput(input);
    input.resize(13);  // NOLINT TODO(ashish): change
  }
}