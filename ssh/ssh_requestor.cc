#include "ssh_requestor.h"

#include <glog/logging.h>

#include "../protocol/protocol_adapter.h"

namespace scom {

SshRequestor::SshRequestor(
    const std::string& host,
    const std::string& user,
    const std::string& command)
    : ssh_initator_(host, user, command, kUsePublicKeyAuth) {}

void SshRequestor::Send(const std::string& message) {
  msg_io_mgr_.Send(message, ssh_initator_);
}

std::string SshRequestor::Receive() {
  return std::move(msg_io_mgr_.Receive(ssh_initator_));
}

}  // namespace scom