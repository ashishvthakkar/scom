#include "ssh_initiator.h"

int main() {
  SshInitiator(
      "ash-alienw-r11",
      "ashish",
      "/home/ashish/git/scom/build/ssh/ssh_responder -request=ping");
}