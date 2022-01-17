#include "ssh_requestor.h"

SshRequestor::SshRequestor(
    const std::string& host,
    const std::string& user,
    const std::string& command)
    : ssh_initator_(SshInitiator(host, user, command)),
      request_id_(0) {}

std::string SshRequestor::SendReceive(const std::string& request) {
  scom::WriteMessage(kProtocolVersion, request_id_, request, buffer_);
  LOG(INFO) << "Sending pb request with size: " << buffer_.size();
  request_id_++;
  ssh_initator_.Send(buffer_);
  ssh_initator_.Receive(buffer_);

  int version = 0;
  int request_id = 0;
  std::string details;
  scom::ReadMessage(buffer_, version, request_id, details);
  CHECK(version == kProtocolVersion) << "Unexpected version: " << version;
  LOG(INFO) << "Read message with version " << version << ", request id "
            << request_id << " and details: " << std::endl;
  LOG(INFO) << details << std::endl;
  // NOTE: Relying on RVO to optimize the below return
  return details;
}
