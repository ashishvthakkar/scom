#ifndef SCOM_SSH_RESPONDER_H_
#define SCOM_SSH_RESPONDER_H_

#include <glog/logging.h>

#include <fstream>

#include "msg_io_mgr.h"
#include "std_io_mgr.h"

namespace scom {

class SshResponder {
public:
  explicit SshResponder(const std::string& log_file_name);

  void Send(const std::string& message, const int request_id);
  void Send(const std::string& message);
  std::string Receive(int& request_id);
  std::string Receive();

private:
  void ConstructProtobufMessage(
      int version,
      int request_id,
      const std::string& payload);

  StdIoMgr std_io_mgr_;
  MsgIoMgr<StdIoMgr> msg_io_mgr_;
  // Note: Logging controlled by a config parameter kEnableLogging.
  std::ofstream log_file_;
};

}  // namespace scom

#endif