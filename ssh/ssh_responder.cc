#include <iostream>

#include "../protocol/protocol_adapter.h"

int main() {
  std::cout << "Responder started. <eom>" << std::endl;
  std::string input;
  std::cin >> input;
  std::cout << "Received input with size: " << input.size() << std::endl;
  std::cout << "Received input: " << input << std::endl;
  unsigned int version = 0;
  std::string details;
  scom::ReadMessage(version, details, input);
  std::cout << "Read message with version " << version
            << " and details: " << std::endl;
  std::cout << details << std::endl;
  std::cout << "Done. <eom>" << std::endl;
}