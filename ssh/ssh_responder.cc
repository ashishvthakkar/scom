#include <config.h>

#include <fstream>
#include <iostream>

#include "../protocol/protocol_adapter.h"

class SshResponder {
public:
  explicit SshResponder(const std::string& log_file_name) {
    log_file_.open(log_file_name, std::ios_base::app);
    std::cout << "Responder started and will log to " << kResponderLog
              << " <eom>" << std::endl;
  }

  void ProcessInput(const std::string& input) {
    log_file_ << "Received input with size: " << input.size() << std::endl;
    log_file_ << "Received input: " << input << std::endl;
    unsigned int version = 0;
    std::string details;
    scom::ReadMessage(version, details, input);
    std::cout << "Read message with version " << version
              << " and details: " << std::endl;
    std::cout << details << std::endl;
    log_file_ << "Sent resonse for version: " << version << std::endl;
    log_file_ << "Done." << std::endl;
  }

private:
  std::ofstream log_file_;
};

int main() {
  SshResponder ssh_responder(kResponderLog);
  std::string input;
  while (std::cin >> input) {
    ssh_responder.ProcessInput(input);
  }
}