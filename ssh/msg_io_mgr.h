#ifndef SCOM_SSH_MSG_IO_MGR_H_
#define SCOM_SSH_MSG_IO_MGR_H_

#include <glog/logging.h>

#include <fstream>

namespace scom {

template <typename T>
class MsgIoMgr {
public:
  void Send(const std::string& message, const int request_id, T& io_mgr);
  std::string Receive(T& io_mgr, int& request_id);

private:
  void ConstructProtobufMessage(
      int version,
      int request_id,
      const std::string& payload);

  std::string buffer_;
};

}  // namespace scom

#endif