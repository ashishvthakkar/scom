#include <iostream>

int main() {
  std::cout << "Responder started." << std::endl;
  std::string input;
  std::cin >> input;
  std::cout << "Received input: " << input << std::endl;
  std::string input2;
  std::cin >> input2;
  std::cout << "Received new input: " << input2 << std::endl;
  std::cout << "Done." << std::endl;
}