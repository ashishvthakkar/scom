#include "ssh_responder.h"

int main() {
  // SshResponder ssh_responder(kResponderLog);
  std::string input;
  // while (true) {
  //   int next_message_size = ssh_responder.ReadNextMessageSize();
  //   if (next_message_size <= 0) {
  //     break;
  //   }
  //   input.resize(next_message_size);
  //   ssh_responder.ReadNextMessage(input);
  //   ssh_responder.ProcessInput(input);
  // }
  std::ofstream log_file;
  log_file.open(kResponderLog);

  while (std::cin >> input) {
    log_file << "Received input: " << input;
    const std::string response("Response");
    std::cout << response.size();
    std::cout << response;
  }
}