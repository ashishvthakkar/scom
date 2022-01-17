#include "ssh_responder.h"

int main() {
  SshResponder ssh_responder(kResponderLog);
  while (true) {
    int message_size = ssh_responder.GetNextMessageSize();
    if (message_size <= 0) {
      break;
    }
    ssh_responder.HandleNextMessage(message_size);
  }
}