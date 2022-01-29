#include "ssh_responder.h"

#include <config.h>

#include <cstdio>
#include <iostream>

#include "../protocol/protocol_adapter.h"

namespace scom {

SshResponder::SshResponder(const std::string& log_file_name)
    : log_file_(log_file_name),
      std_io_mgr_(log_file_, kEnableResponderLogging) {}

std::string SshResponder::Receive(int& request_id) {
  // NOTE: Relies on RVO
  return msg_io_mgr_.Receive(std_io_mgr_, request_id);
}

std::string SshResponder::Receive() {
  int request_id = 0;  // Not used
  // NOTE: Relies on RVO
  return Receive(request_id);
}

void SshResponder::Send(const std::string& message, const int request_id) {
  msg_io_mgr_.Send(message, request_id, std_io_mgr_);
  if (kEnableResponderLogging) {
    log_file_ << "Sent message.";
  }
}

void SshResponder::Send(const std::string& message) { Send(message, -1); }

}  // namespace scom