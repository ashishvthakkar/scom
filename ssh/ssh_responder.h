#ifndef SCOM_SSH_RESPONDER_H_
#define SCOM_SSH_RESPONDER_H_

#include <glog/logging.h>

#include <fstream>

#include "std_io_mgr.h"

namespace scom {

class SshResponder {
public:
  explicit SshResponder(const std::string& log_file_name);
  std::string Receive();
  void Send(const std::string& message);

private:
  void ConstructProtobufMessage(
      int version,
      int request_id,
      const std::string& payload);

  StdIoMgr std_io_mgr_;
  std::string buffer_;
  // Note: Logging can be controlled by a config parameter for performance.
  std::ofstream log_file_;
};

}  // namespace scom

#endif