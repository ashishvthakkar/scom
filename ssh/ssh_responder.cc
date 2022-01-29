#include "ssh_responder.h"

#include <config.h>

#include <cstdio>
#include <iostream>

#include "../protocol/protocol_adapter.h"

namespace scom {

SshResponder::SshResponder(const std::string& log_file_name)
    : log_file_(log_file_name),
      std_io_mgr_(log_file_, kEnableResponderLogging) {}

std::string SshResponder::Receive() {
  return std::move(msg_io_mgr_.Receive(std_io_mgr_));
}

void SshResponder::Send(const std::string& message) {
  msg_io_mgr_.Send(message, std_io_mgr_);
  if (kEnableResponderLogging) {
    log_file_ << "Sent message.";
  }
}

}  // namespace scom