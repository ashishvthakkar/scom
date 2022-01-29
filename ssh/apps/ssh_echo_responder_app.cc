#include <config.h>
#include <expressions/utils.h>

#include <fstream>

#include "../std_io_mgr.h"

int main() {
  std::ofstream log_file(kResponderLog);
  scom::StdIoMgr io_mgr(log_file, false);
  std::string message;
  while (true) {
    io_mgr.Receive(message);
    if (message.empty()) {
      break;
    }
    // Echo back the request
    io_mgr.Send(message);
  }
  return 0;
}