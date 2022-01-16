#include "ssh_requestor.h"

SshRequestor::SshRequestor(
    const std::string& host,
    const std::string& user,
    const std::string& command)
    : ssh_initator_(SshInitiator(host, user, command)),
      request_id_(0) {}

std::string SshRequestor::Send(const std::string& request) {
  // bool SshRequestor::SshResponseHandler(
  //     const char* buffer,
  //     int bytes_read,
  //     ssh::Channel& channel) {
  //   if (bytes_read > 0) {
  //     int version = 0;
  //     int request_id = 0;
  //     std::string payload;
  //     scom::ReadMessage(buffer, version, request_id, payload);
  //     CHECK(version == kProtocolVersion) << "Unexpected version: " <<
  //     version; LOG(INFO) << "For request " << request_id << ", got payload: "
  //     << payload;
  //   }
  //   if (next_request_id_ >= requests_.size()) {
  //     return true;  // done
  //   }
  LOG(INFO) << "Sending request: " << request;
  std::string protobuf_request;
  scom::WriteMessage(kProtocolVersion, request_id_, request, protobuf_request);
  protobuf_request.append("\n");
  request_id_++;
  std::string protobuf_response;
  protobuf_response.resize(kBufferSize);
  ssh_initator_.Send(protobuf_request, protobuf_response);
  // NOTE: Relying on RVO to optimize the below return
  return protobuf_response;
}
