#include "ssh_requestor.h"

bool SshRequestor::SshResponseHandler(
    const char* buffer,
    int bytes_read,
    ssh::Channel& channel) {
  if (bytes_read > 0) {
    int version = 0;
    int request_id = 0;
    std::string payload;
    scom::ReadMessage(buffer, version, request_id, payload);
    CHECK(version == kProtocolVersion) << "Unexpected version: " << version;
    LOG(INFO) << "For request " << request_id << ", got payload: " << payload;
  }
  if (next_request_id_ >= requests_.size()) {
    return true;  // done
  }
  LOG(INFO) << "Sending request: " << requests_.at(next_request_id_);
  std::string protobuf_request;
  scom::WriteMessage(
      kProtocolVersion,
      next_request_id_,
      requests_.at(next_request_id_),
      protobuf_request);
  protobuf_request.append("\n");
  channel.write(protobuf_request.data(), protobuf_request.size());
  next_request_id_++;
  return false;  // more responses and/or requests to follow
}

SshRequestor::SshRequestor(
    const std::vector<std::string>& requests,
    const std::string& host,
    const std::string& user,
    const std::string& command)
    : requests_(requests),
      next_request_id_(0) {
  SshInitiator(
      host,
      user,
      command,
      [this](const char* buffer, int bytes_read, ssh::Channel& channel) {
        return SshResponseHandler(buffer, bytes_read, channel);
      });
}
