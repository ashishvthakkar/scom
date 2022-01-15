#include <config.h>

#include <fstream>
#include <iostream>

#include "../protocol/protocol_adapter.h"

int main() {
  std::cout << "Responder started and will log to " << kResponderLog << " <eom>"
            << std::endl;
  std::ofstream log_file;
  log_file.open(kResponderLog, std::ios_base::app);
  std::string input;
  std::cin >> input;
  log_file << "Received input with size: " << input.size() << std::endl;
  log_file << "Received input: " << input << std::endl;
  unsigned int version = 0;
  std::string details;
  scom::ReadMessage(version, details, input);
  std::cout << "Read message with version " << version
            << " and details: " << std::endl;
  std::cout << details << std::endl;
  log_file << "Sent resonse for version: " << version << std::endl;
  log_file << "Done." << std::endl;
}