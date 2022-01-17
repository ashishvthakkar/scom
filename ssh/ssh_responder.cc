#include "ssh_responder.h"

#include <config.h>

#include <cstdio>

namespace scom {

SshResponder::SshResponder(const std::string& log_file_name)
    : log_file_(log_file_name),
      std_io_mgr_(log_file_) {}

std::string SshResponder::Receive() {
  std_io_mgr_.Receive(buffer_);
  log_file_ << "Received message with size: " << buffer_.size() << std::endl;
  log_file_ << "Received message: " << buffer_ << std::endl;
  scom::HeaderReadFields header;
  scom::ReadMessage(buffer_, header);
  CHECK(header.version == kProtocolVersion)
      << "Unexpected version: " << header.version;
  log_file_ << "Read message with version " << header.version << ", request id "
            << header.request_id << " and details: " << std::endl;
  log_file_ << header.payload << std::endl;
  return std::move(header.payload);
}

void SshResponder::Send(const std::string& message) {
  // NOTE: The request_id is currently a placeholder only
  const int request_id = -1;
  ConstructProtobufMessage(kProtocolVersion, request_id, message);
  std_io_mgr_.Send(buffer_);
  log_file_ << "Sent pb response with size: " << buffer_.size() << std::endl;
  log_file_ << "Sent pb response: " << buffer_ << std::endl;
  log_file_ << "Done with request.";
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
  log_file_ << "Protobuf output: " << buffer_ << std::endl;
}

}  // namespace scom