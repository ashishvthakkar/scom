#include "ssh_requestor.h"

#include <glog/logging.h>

#include "../protocol/protocol_adapter.h"

namespace scom {

SshRequestor::SshRequestor(
    const std::string& host,
    const std::string& user,
    const std::string& command)
    : ssh_initator_(host, user, command, kUsePublicKeyAuth) {}

void SshRequestor::Send(const std::string& message, const int request_id) {
  msg_io_mgr_.Send(message, request_id, ssh_initator_);
}

void SshRequestor::Send(const std::string& message) { Send(message, -1); }

std::string SshRequestor::Receive(int& request_id) {
  // NOTE: Relies on RVO
  return msg_io_mgr_.Receive(ssh_initator_, request_id);
}

std::string SshRequestor::Receive() {
  int request_id = 0;  // Not used
  // NOTE: Relies on RVO
  return Receive(request_id);
}

}  // namespace scom