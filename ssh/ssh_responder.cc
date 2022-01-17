#include "ssh_responder.h"

#include <config.h>

#include <cstdio>
#include <iostream>

#include "../protocol/protocol_adapter.h"

namespace scom {

SshResponder::SshResponder(const std::string& log_file_name)
    : log_file_(log_file_name),
      std_io_mgr_(log_file_) {}

std::string SshResponder::Receive() {
  std_io_mgr_.Receive(buffer_);
  scom::HeaderReadFields header;
  scom::ReadMessage(buffer_, header);
  CHECK(header.version == kProtocolVersion)
      << "Unexpected version: " << header.version;
  log_file_ << "Read message with version " << header.version << ", request id "
            << header.request_id << std::endl;
  return std::move(header.payload);
}

void SshResponder::Send(const std::string& message) {
  // NOTE: The request_id is currently a placeholder only
  const int request_id = -1;
  ConstructProtobufMessage(kProtocolVersion, request_id, message);
  std_io_mgr_.Send(buffer_);
  log_file_ << "Sent message.";
}

void SshResponder::ConstructProtobufMessage(
    int version,
    int request_id,
    const std::string& payload) {
  scom::HeaderWriteFields header{
      .version = version,
      .request_id = request_id,
      .payload = payload};
  scom::WriteMessage(header, buffer_);
}

}  // namespace scom