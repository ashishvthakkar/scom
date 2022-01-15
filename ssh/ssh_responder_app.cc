#include "ssh_responder.h"

int main() {
  SshResponder ssh_responder(kResponderLog);
  std::string input;
  while (std::cin >> input) {
    ssh_responder.ProcessInput(input);
  }
}