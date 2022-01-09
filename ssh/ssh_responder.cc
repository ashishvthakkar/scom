#include <gflags/gflags.h>

#include <iostream>

DEFINE_string(request, "", "[ping]");  // NOLINT

int main(int argc, char **argv) {
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  std::cout << "Got request: " << FLAGS_request << std::endl;
  // std::string input;
  // std::cin >> input;
  // std::cout << "Received input: " << input << std::endl;
  if (FLAGS_request == "ping") {
    std::string response("pong");
    std::cout << "Responding with: " << response << std::endl;
  }
  std::cout << "Done." << std::endl;
}